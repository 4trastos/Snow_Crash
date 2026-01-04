
# SnowCrash - Level 01: Debilidad en el Hash de `/etc/passwd`

Este repositorio documenta el proceso de resolución del **Nivel 01** del proyecto SnowCrash. El reto consiste en identificar una vulnerabilidad crítica en la gestión de usuarios y romper un hash expuesto.

## 1. Análisis Inicial

### Identificación del Objetivo e Inseguridad

Tras acceder al sistema como `level01`, se procedió a inspeccionar el archivo de configuración de usuarios:

```bash
cat /etc/passwd | grep "flag01"

```

**Resultado:**
`flag01:42hDRfypTqqnw:3001:3001:home/user/flag01:/bin/bash`

**Interpretación de datos:**

* **Vulnerabilidad detectada:** El segundo campo, que debería contener una `x` (indicando que el hash está protegido en `/etc/shadow`), contiene directamente el hash **42hDRfypTqqnw**.
* **Tipo de Hash:** Se identifica un hash de tipo **DES** (Data Encryption Standard), el formato tradicional de criptografía en Unix antiguos.
* **UID 3001:** Confirmamos que el usuario objetivo es `flag01`.

---

## 2. Conceptos de Criptografía: ¿Qué es el Hash?

Un hash es una función unidireccional que transforma un texto plano en una cadena de longitud fija.

* **Irreversibilidad:** No se puede "desencriptar". La única forma de obtener la contraseña original es mediante ataques de comparación.
* **Salt (Sal):** En este caso, los dos primeros caracteres del hash (`42`) actúan como "sal" para modificar el resultado del algoritmo y dificultar ataques precalculados.

---

## 3. Herramienta Utilizada: John the Ripper

Para romper el hash expuesto, se utilizó **John the Ripper**, una herramienta líder en auditoría de contraseñas que permite realizar ataques de diccionario y fuerza bruta de forma eficiente.

### Proceso de Crackeo

1. **Preparación:** Se volcó la línea del usuario al archivo `pass.txt`.
2. **Ejecución:** Se lanzó la herramienta para identificar la contraseña:

```bash
john pass.txt

```

**Salida de la herramienta:**

```text
Loaded 1 password hash (descrypt, traditional crypt(3) [DES 128/128 SSE2])
abcdefg          (flag01)
1g 0:00:00:00 100% 33.33g/s 1133Kp/s 1133Kc/s 1133KC/s

```

3. **Verificación:** Se confirmó el hallazgo con el comando `--show`:

```bash
john --show pass.txt
# flag01:abcdefg:3001:3001::/home/flag/flag01:/bin/bash

```

---

## 4. Resultado y Resolución

Con la contraseña obtenida, se procedió a la escalada de privilegios y obtención del token de victoria.

* **Contraseña crackeada:** `abcdefg`

### Pasos finales:

1. Cambiar al usuario objetivo: `su flag01`
2. Introducir la contraseña: `abcdefg`
3. Ejecutar el binario de validación: `getflag`

```text
level01@SnowCrash:~$ su flag01
Password:
Don't forget to launch getflag !
flag01@SnowCrash:~$ getflag
Check flag.Here is your token : f2ar5il02puano7naaf6adaaf
flag01@SnowCrash:~$ su level02
Password: [Token de f2ar5il...]

```

**Level 01 passed!**

---

**Logros técnicos:**

* Identificación de malas configuraciones de seguridad en `/etc/passwd`.
* Comprensión de funciones hash y el algoritmo DES.
* Uso de **John the Ripper** para ataques de diccionario/fuerza bruta.

---