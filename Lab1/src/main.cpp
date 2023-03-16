#include <Arduino.h>
#include <Wire.h>
#include "AHT20.h"
AHT20 aht20;

void setup() {
  Serial.begin(9600);
  Serial.println("AHT20 demo!");

  aht20.begin();
}

void loop() {
  float temperature, humidity;
  aht20.getSensors(&humidity,&temperature);
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println("% rH");

  delay(1000);
}