# Ejercicio 2 - Reto 5 - Arduino

Este proyecto corresponde al **Reto 5** de ejercicios con Arduino.  
Incluye un diagrama del circuito, el código fuente en Arduino y un enlace al diseño en Tinkercad.  

---

## 📂 Archivos del proyecto

- `reto5.ino` → Código en Arduino.
- `assets/reto5.png` → Imagen del circuito.
- `README.md` → Documentación del proyecto.

---

## 📝 Descripción

### Reto 5: Sistema de alarma con múltiples interrupciones

- Un sensor (ej. PIR o un pulsador) conectado al pin 2 genera una interrupción externa al detectar movimiento.
- Al activarse, empieza a sonar un buzzer con frecuencia variable usando interrupción por Timer2.
- La alarma se detiene al presionar otro botón conectado al pin 3 (INT1).
- Objetivo: Integrar múltiples fuentes de interrupción (externas e internas) en un sistema con diferentes estados.

El diseño se puede visualizar tanto en la imagen incluida como en la simulación en Tinkercad.

---

## 🖼️ Circuito

![Circuito Reto 5](./assets/reto5.png)

---

## 🔗 Simulación en Tinkercad

Puedes ver y simular el circuito en el siguiente enlace:  
👉 [Abrir en Tinkercad](https://www.tinkercad.com/things/9EcpnBPHTuj-reto25)

---

✍️ Autor: Danny
