# Ejercicio 4 - Reto 3 - Arduino

Este proyecto corresponde al **Reto 3** de ejercicios con Arduino.  
Incluye un diagrama del circuito, el código fuente en Arduino y un enlace al diseño en Tinkercad.  

---

## 📂 Archivos del proyecto

- `reto3.ino` → Código en Arduino.
- `assets/reto3.png` → Imagen del circuito.
- `README.md` → Documentación del proyecto.

---

## 📝 Descripción

### Reto 3: Sincronización con semáforos
Objetivo: 
- Introducir el control de acceso a recursos compartidos.


- Dos tareas intentan escribir en la pantalla LCD o en el puerto Serial (recurso compartido).
- Usar un semáforo binario o un mutex para que solo una tarea acceda al recurso a la vez.
- Mostrar en la salida Serial qué tarea está escribiendo y verificar que no haya mensajes mezclados.


El diseño se puede visualizar tanto en la imagen incluida como en la simulación en Tinkercad.

---

## 🖼️ Circuito

![Circuito Reto 3](./assets/reto3.jpeg)

---

✍️ Autor: Danny
