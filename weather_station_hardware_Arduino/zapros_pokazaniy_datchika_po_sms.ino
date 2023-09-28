/* 
Тестировалось на Arduino IDE 1.8.5
Дата тестирования 21.06.2019г.
*/ 

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include  <Adafruit_Sensor.h>
#define DHTTYPE DHT22
#define BMP_SDA 21
#define BMP_SCL 22

Adafruit_BMP280 bmp280;
const int dhtPin = 32;
DHT dht(dhtPin, DHTTYPE);
const int flamePin = 13;
const int raindropPin = 14;
const int lightPin = 27;
const int soilMoisturePin = 34;
const int ammoniaPin = 35;
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(flamePin, INPUT);
  pinMode(raindropPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(soilMoisturePin, INPUT);
  pinMode(ammoniaPin, INPUT);
  boolean status = bmp280.begin(0x76);
  dht.begin();
  delay(7000);
  test_sim800_module();
  send_SMS();
}
void loop() {
  updateSerial();
}
void test_sim800_module()
{
  Serial2.println("AT");
  updateSerial();
  Serial.println();
  Serial2.println("AT+CSQ");
  updateSerial();
  Serial2.println("AT+CCID");
  updateSerial();
  Serial2.println("AT+CREG?");
  updateSerial();
  Serial2.println("ATI");
  updateSerial();
  Serial2.println("AT+CBC");
  updateSerial();
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
void send_SMS()
{
  float humidity = dht.readHumidity();
  float temperatureCelsius = dht.readTemperature();

  float temp = bmp280.readTemperature();
  float press = bmp280.readPressure() / 100;

  int rawValue = analogRead(raindropPin);
  
 
  if (isnan(humidity) || isnan(temperatureCelsius)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } 
  int flameValue = analogRead(flamePin);
  int flameValue1 = map(flameValue, 0, 4095, 1, 0);
  int rainValue = analogRead(raindropPin);
  int rainValue1 = map(rainValue, 2000, 4095, 100, 0);
  int lightValue = analogRead(lightPin);
  int lightValue1 = map(lightValue, 0, 4095, 100, 0);
  int soilMoistureValue = analogRead(soilMoisturePin);
  int soilMoistureValue1 = map(soilMoistureValue, 2330, 3300, 100, 0);
  int ammoniaValue = analogRead(ammoniaPin);

  Serial2.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  Serial2.println("AT+CMGS=\"+77718885212\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms (измените телефонный номер и код страны на тот, который вам нужен)
  updateSerial();
  Serial2.print("TEMP:  " + String(temperatureCelsius) + "   VLAG:  " + String(humidity) + "   RAIN:  " + String(rainValue1) + "   LIGHT:  " + String(lightValue1) + "   DAVLENIE:  " + String(press) + "   AMMIAK:  " + String(ammoniaValue)); //text content
  updateSerial();
Serial.println();
  Serial.println("Message Sent");
  Serial2.write(26);
}