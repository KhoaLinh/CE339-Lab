#include <Arduino.h>
#include <Wire.h>
#include "AHT20.h"
AHT20 aht20;

int ledPin = A0; // chan dieu khien LED
float A = 24; // mốc giá trị A
float B = 26; // mốc giá trị B
float C = 28; // mốc giá trị C

void setup() {
  Serial.begin(9600);
  Serial.println("AHT20 demo!");
  pinMode(ledPin, OUTPUT);

  aht20.begin();
}

void activateLED( int dutyCycle, int period, int duration) {
  int onTime = (dutyCycle * period) / 100;
  int offTime = period - onTime;
  int startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(ledPin, HIGH);
    delay(onTime);
    digitalWrite(ledPin, LOW);
    delay(offTime);
  }
}

void loop() {
  float temperature, humidity;
  aht20.getSensors(&humidity,&temperature);
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println("% rH");
  if (temperature <= A) {
    activateLED(25, 1000, 10000);
  }
  else if (temperature > A && temperature <= B) {
    activateLED(40, 1000, 30000);
  }
  else if (temperature > B && temperature <= C) {
    activateLED(60, 1000, 45000);
  }
  else if (temperature > C) {
    activateLED(75, 1000, 60000);
  }
}