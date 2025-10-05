/*
  Alarma IoT ESP32 con EEPROM externa 24C08 (EEPROM1 = logs, EEPROM2 inactiva)
  Sensor E18-D80NK NPN
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

// -------------------- PINS --------------------
#define PIN_PIR       34
#define PIN_BUTTON    25
#define PIN_LED       26
#define PIN_BUZZER    27

// -------------------- EEPROM --------------------
#define EEPROM1_I2C_ADDR 0x50
#define EEPROM_PAGE_SIZE 16
#define EEPROM1_SIZE_BYTES 1024UL

#define ADDR2_EVENTS      0x010
#define EVENT_RECORD_SIZE 16
#define MAX_EVENTS1 ((EEPROM1_SIZE_BYTES - ADDR2_EVENTS)/EVENT_RECORD_SIZE)

// -------------------- DEFAULT CONFIG --------------------
const char* SSID_DEFAULT = "HONOR 200 Smart";
const char* PASS_DEFAULT = "maso35910";
const char* MQTT_SERVER_DEFAULT = "53dcf012fbbd49d88f4ffcbcff41a6c3.s1.eu.hivemq.cloud";
const int MQTT_PORT_DEFAULT = 8883;
const char* MQTT_USER_DEFAULT = "Oppenheimer";
const char* MQTT_PASS_DEFAULT = "Danteesp32";

// -------------------- MQTT topics --------------------
const char* TOPIC_ALARM_STATUS  = "alarma/status";
const char* TOPIC_ALARM_EVENT   = "alarma/evento";
const char* TOPIC_CONTROL       = "alarma/control";
const char* TOPIC_EEPROM1_READ  = "alarma/eeprom1/read";

// -------------------- Globals --------------------
WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

volatile bool pirFlag = false;
volatile bool btnFlag = false;

bool alarmArmed = true;
bool alarmTriggered = false;
bool alarmForced = false;

TaskHandle_t taskMQTTHandle = NULL;
TaskHandle_t taskAlarmHandle = NULL;
TaskHandle_t taskLEDHandle = NULL;

// -------------------- EEPROM1 (logs) --------------------
uint16_t evtWritePtr1 = 0;

void eepromWriteBytes(uint8_t devaddr, uint16_t addr, const uint8_t* data, size_t len) {
  size_t written = 0;
  while (written < len && addr < EEPROM1_SIZE_BYTES) {
    uint8_t pageOffset = addr % EEPROM_PAGE_SIZE;
    uint8_t spaceInPage = EEPROM_PAGE_SIZE - pageOffset;
    uint8_t toWrite = min((size_t)(len - written), (size_t)spaceInPage);

    Wire.beginTransmission(devaddr);
    Wire.write((uint8_t)(addr & 0xFF));
    Wire.write(data + written, toWrite);
    Wire.endTransmission();
    delay(6);

    written += toWrite;
    addr += toWrite;
  }
}

void eepromReadBytes(uint8_t devaddr, uint16_t addr, uint8_t* data, size_t len) {
  if (addr >= EEPROM1_SIZE_BYTES) return;
  for (size_t i = 0; i < len && (addr+i) < EEPROM1_SIZE_BYTES; i++) {
    Wire.beginTransmission(devaddr);
    Wire.write((uint8_t)((addr+i)&0xFF));
    Wire.endTransmission();
    Wire.requestFrom((int)devaddr,1);
    if(Wire.available()) data[i] = Wire.read();
    else data[i] = 0xFF;
  }
}

void pushEventToEEPROM1(uint8_t type, uint32_t millisTS) {
  uint8_t rec[EVENT_RECORD_SIZE];
  memset(rec,0,EVENT_RECORD_SIZE);
  rec[0] = type;
  rec[1] = (millisTS >> 24) & 0xFF;
  rec[2] = (millisTS >> 16) & 0xFF;
  rec[3] = (millisTS >> 8)  & 0xFF;
  rec[4] = millisTS & 0xFF;

  uint32_t addr = ADDR2_EVENTS + (uint32_t)evtWritePtr1*EVENT_RECORD_SIZE;
  eepromWriteBytes(EEPROM1_I2C_ADDR, addr, rec, EVENT_RECORD_SIZE);
  evtWritePtr1 = (evtWritePtr1 + 1) % MAX_EVENTS1;
}

// -------------------- ISR --------------------
void IRAM_ATTR pirISR() { pirFlag = true; }
void IRAM_ATTR btnISR() { btnFlag = true; }

// -------------------- Helpers --------------------

void publishAlarmStatus(const char* status) {
  mqttClient.publish(TOPIC_ALARM_STATUS,status);

  // Hacer parpadear el LED según estado
  if(strcmp(status,"ARMADA") == 0) {
    for(int i=0;i<2;i++){
      digitalWrite(PIN_LED,HIGH);
      delay(250);
      digitalWrite(PIN_LED,LOW);
      delay(250);
    }
  }
  else if(strcmp(status,"DESARMADA") == 0) {
    for(int i=0;i<4;i++){
      digitalWrite(PIN_LED,HIGH);
      delay(100);
      digitalWrite(PIN_LED,LOW);
      delay(100);
    }
  }
}

void publishAlarmEvent(const char* event)  { mqttClient.publish(TOPIC_ALARM_EVENT,event); }

// -------------------- MQTT CALLBACK --------------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for(unsigned int i=0; i<length; i++) msg += (char)payload[i];

  // --- Control de alarma ---
  if(String(topic) == TOPIC_CONTROL){
    if(msg == "ARMAR"){ 
      alarmArmed = true; 
      alarmTriggered = false; 
      alarmForced = false; 
      publishAlarmStatus("ARMADA"); 
    }
    if(msg == "DESARMAR"){ 
      alarmArmed = false; 
      alarmTriggered = false; 
      alarmForced = false; 
      publishAlarmStatus("DESARMADA"); 
    }
    if(msg == "FORZAR"){ 
      alarmForced = true; 
      publishAlarmStatus("FORZADA"); 
    }
  }

  // --- Lectura de EEPROM ---
  if(String(topic) == TOPIC_EEPROM1_READ){
    StaticJsonDocument<512> doc;  // Declarar aquí
    static uint16_t lastPublishedIdx = 0;
    int count = 0;
    int idx = lastPublishedIdx;

    while(idx != evtWritePtr1 && count < 10){
      uint8_t rec[EVENT_RECORD_SIZE];
      eepromReadBytes(EEPROM1_I2C_ADDR, ADDR2_EVENTS + idx*EVENT_RECORD_SIZE, rec, EVENT_RECORD_SIZE);
      if(rec[0] != 0){
        JsonObject o = doc.createNestedObject();
        o["tipo"] = rec[0];
        o["ts"]   = ((uint32_t)rec[1]<<24)|((uint32_t)rec[2]<<16)|((uint32_t)rec[3]<<8)|rec[4];
        count++;
      }
      idx = (idx + 1) % MAX_EVENTS1;
    }

    lastPublishedIdx = evtWritePtr1;

    if(doc.size() > 0){   // Solo publicar si hay eventos
      String out;
      serializeJson(doc, out);
      mqttClient.publish(TOPIC_EEPROM1_READ, out.c_str());
    }
  }
}

// -------------------- Tasks --------------------
void TaskMQTT(void* pv){
  for(;;){
    if(!mqttClient.connected()){
      Serial.println("Intentando conectar MQTT...");
      String clientId = "ESP32AlarmClient-";
      clientId += String((uint32_t)ESP.getEfuseMac(),HEX);
      if(mqttClient.connect(clientId.c_str(), MQTT_USER_DEFAULT, MQTT_PASS_DEFAULT)){
        Serial.println("MQTT conectado!");
        mqttClient.subscribe(TOPIC_CONTROL);
        mqttClient.subscribe(TOPIC_EEPROM1_READ);
      } else {
        Serial.print("MQTT fallo, reintentando en 2s... Estado: "); Serial.println(mqttClient.state());
        vTaskDelay(2000/portTICK_PERIOD_MS);
      }
    }
    mqttClient.loop();
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void TaskAlarm(void* pv){
  int lastSensor=-1;
  unsigned long lastBtnMillis = 0;        // <-- variable para anti-rebote
  const unsigned long debounceDelay = 200; // 200 ms de anti-rebote

  for(;;){
    if(btnFlag){ 
      btnFlag=false; 
      unsigned long now = millis();
      if(now - lastBtnMillis > debounceDelay){   // <-- anti-rebote
        alarmArmed = !alarmArmed; 
        alarmTriggered = false; 
        alarmForced = false; 
        publishAlarmStatus(alarmArmed ? "ARMADA" : "DESARMADA"); 
        lastBtnMillis = now;                       // <-- actualizar tiempo
      }
    }

    if(pirFlag){ 
      pirFlag=false;
      int val=digitalRead(PIN_PIR);
      if(val==LOW && alarmArmed && !alarmForced){
        alarmTriggered=true;
        publishAlarmEvent("INTRUSION");
        pushEventToEEPROM1(0x03,millis());
        publishAlarmStatus("INTRUSION");
      } else publishAlarmEvent("DETECCION");
    }

    int sensorVal=digitalRead(PIN_PIR);
    if(sensorVal!=lastSensor){ 
      lastSensor=sensorVal; 
      mqttClient.publish("alarma/sensor/status",sensorVal==LOW?"OBJETO":"SIN OBJETO"); 
    }

    vTaskDelay(200/portTICK_PERIOD_MS);
  }
}

void TaskLED(void* pv){
  for(;;){
    if(alarmForced || alarmTriggered){ 
      digitalWrite(PIN_LED,HIGH); 
      digitalWrite(PIN_BUZZER,HIGH); 
    }
    else{ 
      digitalWrite(PIN_LED,LOW); 
      digitalWrite(PIN_BUZZER,LOW); 
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

// -------------------- setup --------------------
void setup(){
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);

  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_PIR,INPUT);
  pinMode(PIN_BUTTON,INPUT_PULLUP);
  pinMode(PIN_BUZZER,OUTPUT);

  // Conectar WiFi
  Serial.print("Conectando WiFi: "); Serial.println(SSID_DEFAULT);
  WiFi.begin(SSID_DEFAULT,PASS_DEFAULT);
  unsigned long start = millis();
  while(WiFi.status()!=WL_CONNECTED && millis()-start<10000){ Serial.print("."); delay(500); }
  if(WiFi.status()==WL_CONNECTED){ Serial.println("\nWiFi conectado!"); Serial.print("IP: "); Serial.println(WiFi.localIP()); }
  else Serial.println("\nFallo al conectar WiFi");

  // MQTT
  wifiClient.setInsecure();
  mqttClient.setServer(MQTT_SERVER_DEFAULT,MQTT_PORT_DEFAULT);
  mqttClient.setCallback(mqttCallback);

  attachInterrupt(digitalPinToInterrupt(PIN_PIR), pirISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), btnISR, FALLING);

  xTaskCreatePinnedToCore(TaskMQTT,"TaskMQTT",4096,NULL,3,&taskMQTTHandle,1);
  xTaskCreatePinnedToCore(TaskAlarm,"TaskAlarm",4096,NULL,4,&taskAlarmHandle,1);
  xTaskCreatePinnedToCore(TaskLED,"TaskLED",2048,NULL,2,&taskLEDHandle,1);
}

// -------------------- loop --------------------
void loop(){ vTaskDelay(1000/portTICK_PERIOD_MS); }
