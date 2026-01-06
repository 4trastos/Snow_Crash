# SnowCrash - Level 09: Cifrado de Rotación Progresiva

Este repositorio documenta la resolución del **Nivel 09**. El reto consiste en revertir un algoritmo de ofuscación de caracteres aplicado a un archivo de autenticación.

## 1. Análisis Inicial

Al listar el directorio, encontramos un binario con bit SUID y un archivo llamado `token`:

```bash
ls -la
# -rwsr-sr-x 1 flag09  level09 7640 Mar  5  2016 level09
# ----r--r-- 1 flag09  level09   26 Mar  5  2016 token

```

### Comportamiento del Binario

Al probar el binario con una entrada conocida, descubrimos el patrón de cifrado:

* Entrada: `aaaaaaaa`
* Salida: `abcdefgh`

**Deducción del Algoritmo:**
El programa aplica un cifrado de rotación donde el valor sumado a cada carácter es igual a su índice (posición) en la cadena:


---

## 2. Ingeniería Inversa y Limitaciones

Intentamos analizar el binario con `ltrace`, pero descubrimos una protección **anti-debugging**:

```text
ptrace(PTRACE_TRACEME, ...) = -1
"You should not reverse this"

```

El binario utiliza `ptrace` para detectar si está siendo trazado y abortar la ejecución. Sin embargo, dado que ya hemos deducido el algoritmo mediante "caja negra" (probando entradas y salidas), el análisis dinámico no es estrictamente necesario.

---

## 3. Implementación del Exploit (C)

El archivo `token` contiene caracteres no imprimibles, lo que impide pasarlo como argumento de shell sin corromper los datos. La solución es desarrollar un programa en C que lea el archivo directamente byte a byte y aplique la operación inversa:


### Código del Decodificador (`destroyer.c`)

```c
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main (int argc, char **argv)
{
    char    buffer[BUFFER_SIZE];
    char    c;
    int     i;
    int     fd;
    ssize_t bytes;

    if (argc != 2)
    {
        printf("Error: ( %s ): Necesita argumento\n", argv[0]);
        return (1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        return (1);

    bytes = read(fd, buffer, BUFFER_SIZE - 1);
    if (bytes == -1)
    {
        printf("Error al leer el archivo\n");
        close(fd);
        return (1);
    }

    buffer[bytes] = '\0';

    i = 0;
    while (buffer[i] != '\0')
    {
        c = buffer[i] - i;
        printf("%c", c);
        i++;
    }
    close(fd);
    printf("\n");
    return (0);
}

```

---

## 4. Resultado y Resolución

Ejecutamos nuestro decodificador sobre el archivo `token` original de la VM:

```bash
./a.out token
# Salida: f3iji1ju5yuevaus41q1afiuq

```

### Escalada Final:

1. **Cambio a flag09:** `su flag09`
2. **Contraseña:** `f3iji1ju5yuevaus41q1afiuq`
3. **Obtención del flag:** `getflag`

* **Token obtenido:** `s5cAJpM8ev6XHw998pRWG728z`

**Level 09 passed!**

---

## 5. Conclusiones Técnicas

* **Criptografía Débil:** Un cifrado basado únicamente en la posición del carácter es vulnerable a ataques de texto plano conocido.
* **Anti-Debugging:** El uso de `ptrace` es una técnica común para dificultar el análisis, pero no protege contra la deducción lógica del algoritmo.
* **Manipulación de Datos Binarios:** Leer archivos directamente en C es la forma más fiable de procesar datos que contienen caracteres no ASCII.

---