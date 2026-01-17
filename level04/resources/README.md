
# SnowCrash - Level 04: Inyección de Comandos en Script CGI Perl

Este repositorio documenta el proceso de resolución del **Nivel 04** de SnowCrash. El reto consiste en explotar una vulnerabilidad de inyección de comandos en un script de Perl con privilegios SUID que actúa como un servidor CGI.

## 1. Análisis Inicial

### Identificación del Objetivo

Al inspeccionar el directorio del nivel, encontramos un script de Perl con permisos de ejecución elevados:

```bash
ls -la
# -rwsr-sr-x 1 flag04 level04 152 Mar 5 2016 level04.pl

```

**Observaciones clave:**

* **Bit SUID activo:** El script pertenece a `flag04`, lo que indica que cualquier comando ejecutado por este script podría correr con sus privilegios.
* **Naturaleza del archivo:** Es un script de texto en Perl, lo que nos permite leer su código fuente directamente.

---

## 2. Análisis del Código Fuente

El contenido de `level04.pl` es el siguiente:

```perl
#!/usr/bin/perl
# localhost:4747
use CGI qw{param};
print "Content-type: text/html\n\n";
sub x {
  $y = $_[0];
  print `echo $y 2>&1`;
}
x(param("x"));

```

### Vulnerabilidades Detectadas:

1. **Uso de Backticks (' '):** En Perl, los backticks ejecutan comandos directamente en la shell del sistema.
2. **Falta de Sanitización:** El parámetro `x` se recibe directamente del usuario a través de `param("x")` y se interpola en la cadena de ejecución de `echo` sin ningún filtro.
3. **Inyección de Shell:** Al concatenar `$y` dentro del comando `echo`, un atacante puede insertar metacaracteres de shell (como `;`, `&&` o `$( )`) para ejecutar comandos arbitrarios.

---

## 3. Pruebas y Metodología de Explotación

### Intento 1: Ejecución Local (Desechada)

Se intentó ejecutar el script directamente desde la shell pasando el parámetro por línea de comandos:

```bash
./level04.pl x='$(getflag)'

```

**Resultado:** `Nope there is no token here for you sorry. Try again :)`
**Conclusión:** La ejecución directa local no estaba heredando correctamente los privilegios necesarios o la shell local interfería con la interpretación de la inyección.

### Intento 2: Explotación vía Servicio Web (Éxito)

El script contenía un comentario: `# localhost:4747`. Esto indica que hay un servidor web escuchando en ese puerto y sirviendo el script. Al realizar la petición a través de HTTP, el servidor web gestiona la ejecución del CGI.

**Comando ejecutado:**

```bash
curl 'localhost:4747/level04.pl?x=$(getflag)'

```

**Lógica del Exploit:**

1. `curl` envía una petición GET con el parámetro `x` conteniendo `$(getflag)`.
2. El script recibe `$(getflag)` y lo coloca en la función: ``echo $(getflag) 2>&1``.
3. La shell interpreta `$(getflag)`, ejecuta el binario con los privilegios de `flag04` (dueño del script) y sustituye la expresión por el token resultante.
4. El comando final ejecutado es `echo [TOKEN_AQUÍ]`.

---

## 4. Resultado y Resolución

La ejecución a través de `curl` devolvió exitosamente el token de validación:

* **Token obtenido:** `ne2searoevaevoem4ov4ar8ap`

### Pasos finales:

1. Cambiar al usuario objetivo: `su level05`
2. Introducir el token como contraseña: `ne2searoevaevoem4ov4ar8ap`

**Level 04 passed!**

---

## 5. Conclusiones Técnicas

* **Inyección de Comandos:** Nunca se deben pasar variables de usuario directamente a funciones que interactúan con la shell (`system`, `exec`, backticks).
* **Seguridad en Perl:** El uso de CGI requiere una sanitización estricta (usar el modo `taint` en Perl es una buena práctica para evitar esto).
* **Contexto de Ejecución:** A veces, el entorno de ejecución (CLI vs Web Server) altera cómo se procesan los privilegios SUID.

---
