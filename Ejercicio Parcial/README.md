# Proyecto: Comunicación I2C, EEPROM y RTOS

Este proyecto implementa la comunicación mediante el protocolo **I2C** entre un microcontrolador y una memoria **EEPROM**, junto con el uso de un **RTOS** (Sistema Operativo en Tiempo Real) para la organización de tareas.  
Incluye la explicación teórica, la parte física, la simulación, pruebas y resultados.

---

## 1. Introducción

En los sistemas embebidos modernos, la correcta gestión del tiempo y la comunicación entre periféricos es fundamental.  
Este proyecto combina tres elementos importantes:

- **RTOS**: Organización de procesos y tareas en un sistema de tiempo real.  
- **I2C**: Protocolo de comunicación serial síncrona de dos hilos.  
- **EEPROM**: Memoria no volátil para almacenamiento de datos.  

---

## 2. Explicación de los temas

### **EPROM / EEPROM**
- **EEPROM** (Electrically Erasable Programmable Read-Only Memory) es un tipo de memoria no volátil.
- Permite guardar información incluso si se apaga el dispositivo.
- En este proyecto se almacenan datos como configuraciones o valores de sensores.
- Se accede a la EEPROM mediante I2C.

### **RTOS**
- Un **RTOS (Real-Time Operating System)** permite ejecutar varias tareas en paralelo, garantizando tiempos de respuesta.
- Orden de importancia de tareas en RTOS:
  1. **Tareas críticas**: Requieren respuesta inmediata (ej. interrupciones de sensores).
  2. **Tareas de comunicación**: Transferencia de datos (ej. I2C).
  3. **Tareas de almacenamiento**: Guardado en EEPROM.
  4. **Tareas de usuario**: Interfaz o monitoreo.

### **I2C**
- Protocolo de comunicación serial síncrona.
- Solo usa **dos líneas**:
  - **SDA** (Serial Data).
  - **SCL** (Serial Clock).
- El microcontrolador actúa como **maestro** y la EEPROM como **esclavo**.
- Permite enviar y leer datos en secuencia.

---

## 3. Parte Física

- **Microcontrolador**: Configurado como maestro I2C.  
- **EEPROM**: Dispositivo esclavo donde se guardan datos.  
- **Conexiones**:
  - SDA y SCL entre microcontrolador y EEPROM.
  - Resistencias pull-up en SDA y SCL para garantizar la señal.  

📷 Imagen física:  
![Fisico](./assets/Fisico.jpeg)

---

## 4. Simulación y Pruebas

La simulación se realiza en un software de diseño de circuitos (ej. Proteus o Wokwi).  
En la simulación se prueba:  
- Escritura de un valor en la EEPROM.  
- Lectura posterior del mismo valor para verificar que los datos se almacenaron correctamente.  
- Uso de tareas RTOS para manejar los procesos de escritura, lectura y monitoreo.  

📷 Imagen simulación:  
![Simulación](./assets/Simulacion.jpeg)

---

## 5. Resultados

- Se logró la **comunicación estable mediante I2C**.  
- Los datos escritos en la **EEPROM se conservaron** incluso tras reiniciar el microcontrolador.  
- El **RTOS permitió priorizar tareas**, asegurando que la comunicación y almacenamiento fueran confiables.  
- Se validó tanto en la simulación como en el montaje físico.  

---

## Conclusiones

- La integración de **RTOS** con **I2C y EEPROM** facilita el diseño de sistemas embebidos robustos.  
- **EEPROM** es útil para almacenar configuraciones o valores importantes que deben conservarse tras apagar el sistema.  
- El protocolo **I2C** simplifica la conexión entre múltiples dispositivos con solo dos líneas.  
- El uso de **RTOS** asegura que las tareas críticas tengan prioridad y no se vean afectadas por procesos secundarios.  
- Este proyecto demuestra una solución práctica y escalable para sistemas donde la comunicación, almacenamiento y gestión de tiempo real son esenciales.  
