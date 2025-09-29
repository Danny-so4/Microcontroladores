#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

Adafruit_BMP280 bmp;                  // Objeto sensor
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Dirección típica de LCD

void setup() {
  Serial.begin(9600);
  if (!bmp.begin(0x76)) { // Dirección del BMP280
    Serial.println("Error al iniciar BMP280");
    while (1);
  }
  lcd.init();
  lcd.backlight();
}

void loop() {
  float temp = bmp.readTemperature();
  float pres = bmp.readPressure() / 100.0F; // hPa

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Pres: ");
  lcd.print(pres);
  lcd.print(" hPa");

  delay(1000);
}
