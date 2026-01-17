# SnowCrash — Level 09

**Decodificación de un cifrado progresivo aplicado a un archivo protegido**

Este documento describe la resolución del **Nivel 09** siguiendo estrictamente los pasos reales ejecutados durante el proceso.

---

## 1. Análisis Inicial del Entorno

Listado del directorio home:

```bash
ls -la
```

Salida:

```text
dr-x------ 1 level09 level09  140 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level09 level09  220 Apr  3  2012 .bash_logout
-r-x------ 1 level09 level09 3518 Aug 30  2015 .bashrc
-rwsr-sr-x 1 flag09  level09 7640 Mar  5  2016 level09
-r-x------ 1 level09 level09  675 Apr  3  2012 .profile
----r--r-- 1 flag09  level09   26 Mar  5  2016 token
```

Observaciones:

* Existe un binario **SUID** llamado `level09` perteneciente a `flag09`.
* El archivo `token` pertenece a `flag09` y **no es legible** por el usuario actual (`level09`).

---

## 2. Análisis del Binario `level09`

Ejecutamos el binario sin argumentos:

```bash
./level09
```

Salida:

```text
You need to provied only one arg.
```

Ejecutamos el binario con el archivo `token` como argumento:

```bash
./level09 token
```

Salida:

```text
tpmhr
```

Probamos con una entrada controlada:

```bash
./level09 aaaaaaaaa
```

Salida:

```text
abcdefghi
```

### Deducción del algoritmo:

El binario **suma el índice** de cada carácter a su valor ASCII:

* `'a' + 0 → 'a'`
* `'a' + 1 → 'b'`
* `'a' + 2 → 'c'`
* etc.

Es decir, aplica un **cifrado de rotación progresiva**.

---

## 3. Inspección Directa del Archivo `token`

Intentamos leer el archivo directamente:

```bash
cat token
```

Salida:

```text
f4kmm6p|=�p�n��DB�Du{��
```

El contenido incluye **caracteres no imprimibles**, por lo que no puede pasarse correctamente como argumento literal al shell.

Probamos parcialmente:

```bash
./level09 'f4kmm6p|=pnDBDu{'
```

Salida:

```text
f5mpq;v�EyxONQ��
```

Intentamos usar esa salida como contraseña:

```bash
su flag09
Password: f5mpq;vEyxONQ
```

Resultado:

```text
su: Authentication failure
```

Conclusión:
La contraseña está completa en `token`, pero necesitamos **decodificarla correctamente byte a byte**.

---

## 4. Desarrollo del Decodificador en el Host

Dado que el binario suma el índice al carácter, la operación inversa es:

> **restar el índice** a cada carácter.

Creamos el script `decrypt.py` en el host:

```python
import sys

hash = sys.argv[1]
decrypted_hash = ""
for i in range(0, len(hash)):
    decrypted_hash = decrypted_hash + chr(ord(hash[i]) - i)
print(decrypted_hash)
```

Lo copiamos a la VM:

```bash
scp -P 4242 decrypt.py level09@192.168.0.23:/tmp/.
```

---

## 5. Ejecución del Decodificador en la VM

Ejecutamos el script pasando el contenido real de `token`:

```bash
python /tmp/decrypt.py `cat token`
```

Salida:

```text
f3iji1ju5yuevaus41q1afiuq
```

---

## 6. Escalada Final

Usamos la contraseña decodificada:

```bash
su flag09
Password: f3iji1ju5yuevaus41q1afiuq
```

Salida:

```text
Don't forget to launch getflag !
```

Obtenemos la flag:

```bash
getflag
```

Resultado:

```text
Check flag.Here is your token : s5cAJpM8ev6XHw998pRWG728z
```

**Level 09 passed!**

---

## 7. Conclusiones Técnicas

* El sistema utiliza un **cifrado débil**, basado únicamente en la posición del carácter.
* La protección SUID permite leer el archivo `token`, pero no evita su **reversión lógica**.
* El tratamiento de datos binarios requiere **lectura directa del archivo**, no argumentos de shell.
* Este nivel entrena habilidades de:

  * análisis de comportamiento
  * deducción algorítmica
  * manipulación segura de datos binarios
  * inversión de funciones simples

---
