# SnowCrash - Level 00: El Cifrado César

Este repositorio documento el proceso de resolución del **Nivel 00** del proyecto SnowCrash. El reto consiste en descifrar un token oculto en el sistema para obtener la contraseña del usuario `flag00`.

## 1. Análisis Inicial

### Identificación del Objetivo

Antes de proceder con el descifrado, se analizó el archivo `/etc/passwd` para confirmar la estructura de usuarios del reto:

```bash
cat /etc/passwd | grep "00"

```

**Resultado:**
`level00:x:2000:2000::/home/user/level00:/bin/bash`
`flag00:x:3000:3000::/home/flag/flag00:/bin/bash`

**Interpretación de datos:**

* **UID 3000:** Identificamos que el usuario objetivo es `flag00`.
* **Shell:** Ambos usuarios utilizan `/bin/bash`, lo que garantiza una terminal interactiva tras escalar privilegios.
* **Home Directory:** El entorno de `flag00` está segregado en `/home/flag/flag00`, una ruta clave para futuras búsquedas.

---

```
level00@SnowCrash:~$ find / -user flag00 2>/dev/null
/usr/sbin/john
level00@SnowCrash:~$ cat /usr/sbin/john
cdiiddwpgswtgt
level00@SnowCrash:~$ su flag00
Password: cdiiddwpgswtgt
su: Authentification failure
```  

Tras explorar el sistema, se localizó un archivo sospechoso que contenía el siguiente token cifrado:

> **Token:** `cdiiddwpgswtgt`

Al observar que el token solo contiene letras minúsculas y presenta patrones de repetición (`dd`), la hipótesis inicial fue un **Cifrado de Sustitución por Desplazamiento** (César).

## 2. Herramienta Utilizada: `tr`

Para resolverlo, utilizamos el comando `tr` (translate). Este comando permite mapear un conjunto de caracteres a otro. La lógica empleada fue desplazar el abecedario estándar (`a-z`) un número determinado de posiciones.

## 3. Automatización (The Brute Force Script)

En lugar de probar manualmente los 26 desplazamientos posibles, desarrollamos un script en **Bash** para automatizar la "fuerza bruta" y visualizar todas las rotaciones (ROT) de un solo golpe.

### El Script: `decrypt_level00.sh`

```bash
#!/bin/bash

TOKEN=$1
ALPHABET_LOWER="abcdefghijklmnopqrstuvwxyz"
ALPHABET_UPPER="ABCDEFGHIJKLMNOPQRSTUVWXYZ"

echo "Decrypting token: $TOKEN"
echo ""

for ROT in {0..25}
do
    # Crear alfabetos desplazados
    DISP_LOWER="${ALPHABET_LOWER:$ROT}${ALPHABET_LOWER:0:$ROT}"
    DISP_UPPER="${ALPHABET_UPPER:$ROT}${ALPHABET_UPPER:0:$ROT}"

    # Aplicar ROT (minúsculas y mayúsculas)
    DECRYPTED=$(echo "$TOKEN" | tr 'a-z' $DISP_LOWER | tr 'A-Z' $DISP_UPPER)

    # Mostrar resultado
    printf "ROT %02d: %s\n" $ROT "$DECRYPTED"
done

```

## 4. Resultado y Resolución

Al ejecutar el script con el token:

```bash
./decrypt_level00.sh cdiiddwpgswtgt

```

Se obtuvo una lista de 26 variantes. En la posición **ROT 11**, el sistema devolvió una cadena legible:

* **Contraseña descifrada:** `nottoohardhere`

### Pasos finales para obtener la Flag:

1. Cambiar al usuario objetivo: `su flag00`
2. Introducir la contraseña: `nottoohardhere`
3. Ejecutar el binario de validación: `getflag`

```
level00@SnowCrash:~$ su flag00
Password:
Don't forget to launch getflag !
flag00@SnowCrash:~$ getflag
Check flag.Here is your token : x24ti5gi3x0ol2eh4esiuxias
flag00@SnowCrash:~$ su level01
Password:
level01@SnowCrash:~$
```  
Level00 passed !

---

**Logros técnicos:**

* Análisis de criptografía clásica.
* Uso avanzado de tuberías (pipes) en Linux.
* Scripting en Bash para automatización de tareas de seguridad.

---

