## ¿Qué es Snow Crash, en esencia?

**Snow Crash es un wargame de seguridad**.

No es un proyecto de programación tradicional.
Es para dominar la explotación de vulnerabilidades reales adoptando la mentalidad de un atacante mediante razonamiento lógico, paciencia y una metodología sólida.
**romper** programas, configuraciones y sistemas **mal diseñados**.

El objetivo es que:

* Entender **cómo se explotan vulnerabilidades reales**
* Aprender a **pensar como un atacante**
* Desarrollar **razonamiento lógico, paciencia y metodología**

No hay “ejercicios guiados”. Hay **niveles**, y cada nivel es una puerta cerrada.

---

## La idea general del proyecto

* Hay una **máquina virtual Linux** (una ISO específica de Snow Crash).
* Dentro de esa VM hay **usuarios llamados `level00`, `level01`, …**
* Cada nivel contiene **una vulnerabilidad distinta**.
* La misión es:

  1. **Entrar como `levelXX`**
  2. **Conseguir acceso a `flagXX`**
  3. Ejecutar `getflag`
  4. Obtener la **contraseña del siguiente nivel**
  5. Repetir

No hay menú, no hay instrucciones dentro del sistema.
**La VM es el puzzle.**

---

## Flujo real de un nivel

El ciclo siempre es este:

1. Conectarse por SSH como `levelXX`
2. Exploras el sistema:

   * Archivos
   * Permisos
   * Binarios raros
   * Scripts
   * Cron jobs
   * Variables de entorno
   * Servicios abiertos
3. Detectar una **debilidad**
4. Expotarla para:

   * Ejecutar algo como otro usuario
   * Leer algo que no deberías
   * Inyectar comandos
   * Escalar privilegios
5. Consiguir acceso a `flagXX`
6. Ejecutar:

   ```
   getflag
   ```
7. Obtienes el **password del siguiente nivel**

Si no se puede hacer `su flagXX`, el propio subject ya te avisa:

> “find an alternative method, such as a command injection”

Es decir: **la forma “normal” puede estar bloqueada a propósito**.

---

## ¿Qué tipo de cosas se aprenden?

Snow Crash recorre **muchos campos de la seguridad**, por ejemplo:

* Permisos Unix mal configurados
* Setuid / setgid
* Inyección de comandos
* Variables de entorno peligrosas
* Scripts inseguros
* Buffer overflows simples
* Weak passwords / hashes
* Cron jobs explotables
* Archivos temporales inseguros
* PATH hijacking
* Uso incorrecto de funciones estándar

Cada nivel **enseña una idea concreta**.
No es aleatorio. Nada está “porque sí”.

---

## Algo MUY importante: mentalidad

El proyecto insiste mucho en esto (y no es casualidad):

> “You must be the one and only key to the locked doors”

Esto significa:

* Nadie te va a decir *cómo*
* No hay hints automáticos
* Preguntar demasiado pronto va contra el espíritu del proyecto
* La evaluación es **humana** → te harán preguntas

No basta con “funciona”.
Tienes que **entender y explicar**:

* Qué era la vulnerabilidad
* Por qué existe
* Por qué tu solución funciona

---

## Qué NO es Snow Crash

❌ No es brute force
❌ No es copiar comandos de Internet sin entender
❌ No es scripting por scripting
❌ No es “hackear porque sí”

De hecho:

> **Bruteforce está explícitamente prohibido**

---

## La parte del repositorio (MUY importante)

Tu repo **NO contiene el sistema hackeado**.

Contiene:

* Evidencias
* Explicaciones
* Recursos usados
* Scripts auxiliares (si los hiciste)
* Flags (a veces vacíos, pero explicables)

Estructura obligatoria:

```
level00/
  flag
  resources/
level01/
  flag
  resources/
...
```

### ¿Qué va en `resources/`?

Todo lo que **te ayudó a resolver el nivel**, por ejemplo:

* Notas
* Capturas de comandos (copiadas como texto)
* Scripts explicativos
* Diagramas
* Explicaciones paso a paso

⚠️ **No puede haber binarios**
⚠️ Tienes que poder explicar **cada archivo**

---

## Evaluación

En la evaluación:

* Te pedirán que **repliques ataques**
* Te preguntarán:

  * “¿Por qué esto es vulnerable?”
  * “¿Cómo se arreglaría?”
  * “¿Qué aprendiste en este nivel?”
* Puede que tengas que **descargar de nuevo archivos desde la ISO**

Si no sabes explicar algo → **no cuenta**, aunque hayas pasado el nivel.

---

## Parte obligatoria vs bonus

### Obligatorio

* level00 → level09
* Estos **deben estar perfectos**

### Bonus

* level10 → level14
* **Solo cuentan si el obligatorio es impecable**
* Son más duros y más técnicos

---

## Resumen ultra-claro

Snow Crash te pide:

✔ Pensar como atacante
✔ Entender errores de seguridad reales
✔ Explorar sistemas sin pistas
✔ Documentar y explicar TODO
✔ No memorizar comandos, sino conceptos

No es un proyecto rápido.
Es un proyecto que **te cambia la forma de ver Linux y el software**.

---

