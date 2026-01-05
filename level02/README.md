# SnowCrash - Level 02: Análisis Forense de Tráfico de Red (.pcap)

Este repositorio documenta el proceso exhaustivo de resolución del **Nivel 02** de SnowCrash. El reto consiste en interceptar y reconstruir credenciales de usuario a partir de una captura de tráfico de red en un protocolo no cifrado, lidiando con la fragmentación de datos y caracteres de control.

---

## 1. Análisis Inicial y Reconocimiento

Tras acceder al sistema como `level02`, se identificó un archivo sospechoso en el directorio home:

```bash
ls -l ~
# -rwxr-x--- 1 flag02 level02 8302 Aug 30  2015 level02.pcap

```

**Interpretación de datos:**

* **Archivo .pcap:** Se trata de una captura de paquetes de red (Packet Capture).
* **Objetivo:** Extraer información sensible (probablemente un login de Telnet) que viaje en texto plano pero que esté oculta dentro de la estructura binaria del archivo.

---

## 2. Pruebas de Introspección (Desechadas)

Antes de proceder al análisis forense profesional, se realizaron varias pruebas directas en la terminal de la VM para intentar leer el secreto:

### A. Uso de `strings`

```bash
strings level02.pcap

```

* **Resultado:** Muestra fragmentos como `Password:`, `Login incorrect` y la cadena `Nf&Nat`.
* **Deducción:** `Nf&Nat` resultó ser un intento de login fallido. La herramienta `strings` no es capaz de procesar caracteres de control ni de reconstruir el orden real de una sesión fragmentada.

### B. Uso de `cat -v` y `grep`

```bash
cat -v level02.pcap | grep -A 5 "Password"

```

* **Resultado:** Salida binaria con caracteres como `^H` o `^?`.
* **Deducción:** Se identificó que la contraseña no se envió como una cadena simple, sino que contenía instrucciones de teclado (borrados). El `grep` en la VM fallaba al no poder seguir el flujo TCP completo.

### C. Uso de `tcpdump` en la VM

Se intentó leer el contenido ASCII directamente con `tcpdump` para evitar la descarga del archivo:

```bash
tcpdump -q -A -r level02.pcap

```

* **Resultado:** Se visualizaron paquetes individuales con caracteres dispersos.
* **Deducción:** `tcpdump` muestra los paquetes tal cual llegaron a la interfaz. Debido a que Telnet envía casi cada pulsación de tecla en un paquete TCP diferente, reconstruir la contraseña visualmente entre tantas cabeceras de red era propenso a errores humanos.

---

## 3. Fase Forense: Expatriación y Análisis con TShark

Debido a las limitaciones de la VM, se procedió a extraer el archivo y analizarlo con herramientas profesionales de red.

### Paso 1: Copia del archivo al Host local (SCP)

Desde una nueva terminal en la máquina local (fuera de la VM), se ejecutó:

```bash
scp -P 4242 level02@xxx.xxx.xxx.xxx:~/level02.pcap ~/.

```

### Paso 2: Reconstrucción del flujo TCP

Se utilizó **TShark** para realizar un seguimiento del flujo ASCII de la comunicación (Follow TCP Stream), lo que permite ver la sesión tal y como fue enviada por el teclado del usuario:

```bash
tshark -r level02.pcap -q -z follow,tcp,ascii,0

```

**Resultado crítico del volcado:**

```text
Password: 
1 f
1 t
1 _
1 w
1 a
1 n
1 d
1 r
1 .    <-- Carácter de control (BORRAR)
1 .    <-- Carácter de control (BORRAR)
1 .    <-- Carácter de control (BORRAR)
1 N
1 D
1 R
1 e
1 l
1 .    <-- Carácter de control (BORRAR)
1 L
1 0
1 L

```

---

## 4. Deducción y Obtención de la Contraseña Final

A diferencia de un editor de texto normal, el tráfico capturado registra cada pulsación de tecla. El punto (`.`) en la salida de TShark representa el carácter hexadecimal **`0x7f`** (ASCII DEL), que en sesiones de terminal actúa como **Backspace** (Retroceso).

### Proceso de reconstrucción:

1. **Entrada inicial:** `f`, `t`, `_`, `w`, `a`, `n`, `d`, `r`
2. **Instrucciones de borrado:** Aparecen tres caracteres `.` (DEL).
* Se borra la `r`.
* Se borra la `d`.
* Se borra la `n`.
* *Resultado parcial:* `ft_wa`


3. **Nueva entrada:** `N`, `D`, `R`, `e`, `l`
4. **Instrucción de borrado:** Aparece un carácter `.` (DEL).
* Se borra la `l`.
* *Resultado parcial:* `ft_waNDRe`


5. **Entrada final:** `L`, `0`, `L`

**Contraseña Final Cruda:** `ft_waNDReL0L`

---

## 5. Resultado y Resolución

Con la contraseña reconstruida tras el análisis forense, se procedió a la escalada de privilegios.

* **Contraseña obtenida:** `ft_waNDReL0L`

### Pasos finales:

1. Cambiar al usuario objetivo: `su flag02`
2. Introducir la contraseña: `ft_waNDReL0L`
3. Ejecutar el binario de validación: `getflag`

```text
level02@SnowCrash:~$ su flag02
Password: 
Don't forget to launch getflag !
flag02@SnowCrash:~$ getflag
Check flag.Here is your token : kooda2puivaav1idi4f57q8iq
flag02@SnowCrash:~$ su level03
Password: [Token de kooda2...]

```

**Level 02 passed!**

---

**Logros técnicos:**

* Extracción remota de datos mediante **SCP**.
* Análisis de tráfico de red fragmentado y protocolos de texto plano.
* Interpretación de caracteres de control ASCII (`0x7f`) para reconstruir entradas de teclado reales.
* Uso avanzado de **TShark** para el seguimiento de flujos TCP.

