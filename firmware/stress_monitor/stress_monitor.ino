// Physiological stress-response monitor: ESP32 + MAX30102 + DS18B20
// Streams heart rate and peripheral temperature over WebSocket (port 81) every 1.5 s.
// Before flashing, set your own WiFi name and password below. Never commit real credentials.

#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <OneWire.h>
#include <DallasTemperature.h>
 
// Configuración WiFi
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
 
// Pin del DS18B20
#define ONE_WIRE_BUS 4
 
// Instancias de los objetos
MAX30105 particleSensor;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
WebSocketsServer webSocket(81);
 
// Buffer circular para promediar latidos
const byte RATE_SIZE = 20;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute = 0;
float smoothedBpm = 0;
const float SMOOTH_FACTOR = 0.15;
 
// Variables de temperatura
unsigned long lastTempTime = 0;
const unsigned long TEMP_INTERVAL = 2000;
float currentTemp = 0;
 
void resetBPM() {
  beatsPerMinute = 0;
  smoothedBpm = 0;
  for (byte x = 0; x < RATE_SIZE; x++) rates[x] = 0;
  rateSpot = 0;
}
void setup() {
  Serial.begin(115200);
  tempSensor.begin();
 
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 no encontrado");
    while (1);
  }
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP: "); Serial.println(WiFi.localIP());
 
  webSocket.begin();
}
void loop() {
  webSocket.loop();
  long irValue = particleSensor.getIR();
 
  // Detección de latido y cálculo de BPM
  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);
    if (beatsPerMinute > 40 && beatsPerMinute < 200) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;
    }
  }
 
  // Lectura de temperatura cada 2 segundos
  unsigned long now = millis();
  if (now - lastTempTime >= TEMP_INTERVAL) {
    lastTempTime = now;
    tempSensor.requestTemperatures();
    currentTemp = tempSensor.getTempCByIndex(0);
  }
 
  // Envío de datos cada 1.5 segundos
  static unsigned long lastSend = 0;
  if (now - lastSend >= 1500) {
    lastSend = now;
    bool fingerDetected = irValue > 50000;
    if (!fingerDetected) resetBPM();
 
    int displayBpm = 0;
    if (fingerDetected) {
      int validCount = 0, sum = 0;
      for (byte x = 0; x < RATE_SIZE; x++) {
        if (rates[x] > 0) { validCount++; sum += rates[x]; }
      }
      if (validCount >= 5) {
        float currentAvg = (float)sum / validCount;
        if (smoothedBpm == 0) smoothedBpm = currentAvg;
        else smoothedBpm += SMOOTH_FACTOR * (currentAvg - smoothedBpm);
        displayBpm = (int)smoothedBpm;
      }
    }
 
    StaticJsonDocument<128> doc;
    doc["bpm"] = displayBpm;
    doc["temp"] = currentTemp;
    doc["finger"] = fingerDetected;
    doc["raw_ir"] = irValue;
    String json;
    serializeJson(doc, json);
    webSocket.broadcastTXT(json);
  }
}
