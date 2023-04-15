#include <Arduino.h>
#include <Wire.h>
#include "AHT20.h"
#include <WiFiNINA.h>

const char* ssid = "Lab3-20520219";
const char* password = "20520219";

AHT20 aht20;

IPAddress serverIP(192,168,4,1);
int serverPort = 80;

WiFiClient wifiClient;

float temperature, humidity;

void setup() {
  Serial.begin(9600);
  aht20.begin();
  Serial.println();
  Serial.println("Connect to AP ...");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
    Serial.println(WiFi.localIP());
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
    aht20.getSensors(&humidity,&temperature);
    if(isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from AHT20 sensor!");
      return;
    }

    if (wifiClient.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");
    wifiClient.print("Temperature: ");
    wifiClient.print(temperature);
    wifiClient.print(", Humidity: ");
    wifiClient.println(humidity);
    } else {
    Serial.println("Không thể kết nối đến ESP32!");
  }
    delay(2000);
}


  


