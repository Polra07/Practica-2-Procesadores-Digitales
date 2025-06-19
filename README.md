# Práctica: Uso de temporizadores e interrupciones en ESP32 con Arduino

## Objetivo

El propósito de esta práctica es aprender a configurar y utilizar los **temporizadores por hardware** del ESP32 junto con **interrupciones** para ejecutar código periódicamente, sin depender de funciones bloqueantes como `delay()`. Se demuestra cómo capturar eventos de temporizador de forma segura utilizando variables protegidas con `portMUX`.

---

## Descripción del funcionamiento

El código implementa un sistema que genera una **interrupción cada segundo** mediante un temporizador hardware, y lleva un recuento total de las veces que esta interrupción ocurre.

### 1. Variables principales

- `contadorISR`: variable global y **volátil** que se incrementa dentro de la interrupción cada vez que ocurre.
- `totalISR`: lleva el conteo total de las interrupciones ya procesadas en el `loop()`.
- `temporizador`: puntero al temporizador configurado.
- `muxTemporizador`: permite proteger el acceso a variables compartidas entre la ISR y el bucle principal.

---

## Flujo del programa

### `setup()`

1. Inicializa el monitor serie a 115200 baudios.
2. Configura el temporizador hardware 0 con:
   - **Prescaler 80**, lo que da una resolución de 1 µs por tick.
   - **Modo ascendente** (cuenta hacia arriba).
3. Se vincula la función `manejarTemporizador()` como **función de interrupción** (ISR).
4. Se establece una alarma del temporizador que se dispara cada **1.000.000 µs** (1 segundo).
5. Se activa la alarma del temporizador.

### `loop()`

1. Verifica si hay interrupciones pendientes (`contadorISR > 0`).
2. Si hay, se accede de forma segura a la variable compartida `contadorISR` utilizando `portENTER_CRITICAL`.
3. Se decrementa el contador y se incrementa el total.
4. Se imprime un mensaje en el monitor serie con el número total de interrupciones procesadas.

---

## Detalles técnicos

### ¿Por qué usar `volatile`?
La variable `contadorISR` es modificada dentro de una **interrupción**, por lo que debe ser declarada `volatile` para evitar que el compilador optimice su acceso.

### ¿Por qué se usa `portMUX`?
Se utiliza para **proteger el acceso concurrente** a variables compartidas entre la ISR y el `loop()` y así evitar condiciones de carrera.

---

## Salida esperada por el monitor serie

Cada segundo se imprime:

Se ha producido una interrupción. Total: 1
Se ha producido una interrupción. Total: 2
Se ha producido una interrupción. Total: 3

---

## Conclusión

Esta práctica muestra cómo utilizar los **temporizadores por hardware del ESP32** y cómo manejar interrupciones de manera segura. Este enfoque es ideal para aplicaciones en tiempo real donde se requieren acciones periódicas sin bloquear el flujo principal del programa.


