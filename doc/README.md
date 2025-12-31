
# MARCO PROFESIONAL DE ATAQUE (Snow Crash & mundo real)

Piensa Snow Crash no como “16 niveles”, sino como **16 sistemas mal diseñados**.
Tu trabajo es **modelar el sistema**, **entender sus límites**, y **romper sus supuestos**.

---

## 1️⃣ Regla de oro: *Nunca ataques a ciegas*

Antes de tocar nada, **construye un modelo mental**.

Pregúntate SIEMPRE:

> **¿Qué es este sistema?**
> **¿Quién soy yo dentro de él?**
> **Qué puedo tocar y qué no debería poder tocar?**

No ejecutes comandos por ansiedad.
Ejecuta comandos porque **validan hipótesis**.

---

## 2️⃣ Mapa inicial (obligatorio en cada nivel)

Tu primer objetivo no es “hackear”.
Es **mapear el terreno**.

### 🔍 Identidad

* `whoami`
* `id`
* `groups`

👉 Define **tu poder real**, no el aparente.

---

### 📂 Superficie de ataque

* `pwd`
* `ls -la`
* `find . -maxdepth 2 -type f 2>/dev/null`

Clasifica:

* archivos
* binarios
* scripts
* cosas “raras”

No ejecutes aún.

---

### 🔐 Permisos y rarezas

Busca activamente:

* SUID / SGID
* archivos escribibles
* directorios sin protección
* binarios propiedad de otro usuario

> **La mayoría de hacks reales comienzan con permisos, no con exploits.**

---

## 3️⃣ Todo es un contrato mal escrito

Asume que **cada nivel rompe una regla de diseño**.

Ejemplos de reglas rotas:

* “El usuario no puede influir en esto”
* “Este input siempre es seguro”
* “Nadie verá este archivo”
* “El PATH es confiable”
* “El stack no importa”

Tu trabajo es descubrir:

> **¿Qué suposición está haciendo el programador?**

---

## 4️⃣ Si hay un binario, NO lo ejecutes primero

Primero **obsérvalo**.

Orden profesional:

1. `file binary`
2. `ls -l binary`
3. `strings binary`
4. `ldd binary`
5. ¿SUID?
6. ¿Lee input?
7. ¿Accede a archivos?

Ejecutarlo sin entenderlo **es perder control**.

---

## 5️⃣ Modela el flujo de datos (esto es clave)

Pregúntate:

* ¿De dónde viene el input?
* ¿Dónde se transforma?
* ¿Dónde se decide algo importante?
* ¿Dónde termina?

El 90% de los hacks son:

> **Controlar datos antes de una decisión privilegiada**

---

## 6️⃣ Piensa como un atacante, no como un programador limpio

Un atacante piensa:

* ¿Y si esto falla?
* ¿Y si está vacío?
* ¿Y si es enorme?
* ¿Y si contiene `\0`?
* ¿Y si redefine algo existente?

No pienses:

> “Esto no debería pasar”

Piensa:

> “¿Qué pasa si pasa?”

---

## 7️⃣ La creatividad nace de restricciones

No fuerces creatividad.
**Exprímela del entorno**.

Hazte preguntas incómodas:

* ¿Puedo influir en el entorno?
* ¿Puedo cambiar variables?
* ¿Puedo engañar al sistema?
* ¿Puedo ejecutar algo que ejecute otra cosa?

Creatividad =

> **usar algo válido en un contexto inesperado**

---

## 8️⃣ Documenta como si fueras a enseñar esto

Esto es muy importante (y muy profesional).

Para cada nivel:

* qué observaste
* qué hipótesis hiciste
* qué falló
* qué funcionó
* **por qué funcionó**

Eso alimenta:

* intuición
* patrones
* velocidad futura

---

## 9️⃣ Escalabilidad mental (la clave)

Si tus pasos dependen del nivel, estás improvisando.
Si tus pasos son siempre los mismos, estás construyendo **instinto**.

Tu proceso debería ser reutilizable en:

* Snow Crash
* CTFs
* Auditorías reales
* Reverse engineering
* Kernel debugging

---

## 🔟 Señal de que vas bien

Sabes que estás avanzando cuando:

* te detienes antes de ejecutar
* haces menos comandos pero más precisos
* empiezas a **predecir** la vulnerabilidad
* un nivel nuevo te “suena” familiar

---

# 🧭 Mantra profesional (guárdalo)

> **No busco exploits.**
> **Busco suposiciones rotas.**
> **El sistema siempre confía en algo.**
> **Ahí entro yo.**

---


