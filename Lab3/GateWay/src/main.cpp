#include <WiFi.h>

const char* ssid = "Lab3-20520219";
const char* password = "20520219";

int serverPort = 80;

WiFiServer wifiServer(serverPort);
WiFiClient wifiClient;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Địa chỉ IP của ESP32 là: ");
  Serial.println(myIP);
  wifiServer.begin();
  Serial.println("Đang chờ kết nối đến RP2040...");
}

void loop() {
  wifiClient = wifiServer.available();

  if (wifiClient) {
    Serial.println("Đã kết nối thành công đến RP2040!");
    while (wifiClient.connected()) {
      if (wifiClient.available()) {
        String data = wifiClient.readStringUntil('\n');
        Serial.println(data);
      }
    // wifiClient.stop();
    // Serial.println("Đã ngắt kết nối đến RP2040!");
    }
  }
}

