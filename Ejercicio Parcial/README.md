# 🔔 Alarma IoT con ESP32 + EEPROM externa 24C08

Proyecto de una alarma IoT implementada en un **ESP32**, con almacenamiento de eventos en una **EEPROM externa 24C08**, conectividad WiFi y comunicación mediante **MQTT**.  

El sistema utiliza un sensor infrarrojo de proximidad **E18-D80NK (NPN)** para detectar intrusiones, un **LED** y un **buzzer** para señalización, además de un **botón** para armar y desarmar la alarma.  

---

## 🛠️ Componentes

- ESP32 DevKit v1  
- Sensor de proximidad **E18-D80NK (NPN)**  
- EEPROM I2C **24C08**  
- 1 LED + resistencia de 220 Ω  
- 1 pulsador + resistencia de pull-down/pull-up  
- 1 buzzer activo  
- Resistencias varias para el circuito  

---

## 📡 Funcionalidades principales

1. **Detección de intrusiones** con el sensor E18-D80NK.  
2. **Alarma sonora y visual** mediante buzzer y LED.  
3. **Control remoto por MQTT**:
   - `alarma/control` → ARMAR / DESARMAR / FORZAR  
   - `alarma/status` → Estado actual de la alarma  
   - `alarma/evento` → Eventos de detección e intrusión  
   - `alarma/eeprom1/read` → Lectura de logs almacenados en EEPROM  
4. **Logs en EEPROM externa (24C08)** para guardar eventos con marca de tiempo.  
5. **Botón físico** para alternar entre ARMADO y DESARMADO.  
6. **Conexión WiFi + MQTT seguro (TLS/SSL)** con HiveMQ Cloud.  

---

## 📐 Diagramas de conexión

- **Circuito físico**  
  ![Fisico](./assets/Fisico.jpeg)  

- **Simulación (Fritzing)**  
  ![Simulacion](./assets/Simulacion.jpeg)  

### Pines usados en el ESP32

| Componente | Pin ESP32 | Descripción |
|------------|-----------|-------------|
| E18-D80NK  | GPIO34    | Salida sensor (entrada digital) |
| Botón      | GPIO25    | Entrada con interrupción |
| LED        | GPIO26    | Indicador de estado |
| Buzzer     | GPIO27    | Alarma sonora |
| EEPROM SDA | GPIO21    | I2C SDA |
| EEPROM SCL | GPIO22    | I2C SCL |

---

## ⚙️ Configuración por defecto

```cpp
const char* SSID_DEFAULT = "HONOR 200 Smart";
const char* PASS_DEFAULT = "maso35910";

const char* MQTT_SERVER_DEFAULT = "53dcf012fbbd49d88f4ffcbcff41a6c3.s1.eu.hivemq.cloud";
const int   MQTT_PORT_DEFAULT   = 8883;
const char* MQTT_USER_DEFAULT   = "Oppenheimer";
const char* MQTT_PASS_DEFAULT   = "Danteesp32";
