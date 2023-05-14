#include <WiFiNINA.h>
#include <PubSubClient.h>

#define LED_PIN A1

// Wi-Fi Credentials
const char* ssid = "Khoa Linh";
const char* password = "khoalinh1134";

// MQTT Broker Details
const String device_id = "Device1";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_user = "Device0001";
const char* mqtt_password = "Device0001";
const char* mqtt_clientId = "Device_Device0001";
const char* topic_publish = "SensorData";
const char* topic_subscribe = "CommandRequest";

WiFiClient wifi_client;
PubSubClient mqtt_client(mqtt_server, mqtt_port, wifi_client);

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = 10000;

// Variables
byte lightStatus;

void mqtt_publish(const char* data) {
  if (mqtt_client.publish(topic_publish, data)) {
    Serial.println("Published: " + String(data));
  } else {
    Serial.println("Failed to publish");
  }
}

void mqtt_subscribe(const char* topic) {
  if (mqtt_client.subscribe(topic)) {
    Serial.println("Subscribed to: " + String(topic));
  } else {
    Serial.println("Failed to subscribe");
  }
}

void mqtt_connect() {
  while (!mqtt_client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqtt_client.connect(mqtt_clientId, mqtt_user, mqtt_password)) {
      Serial.println("MQTT Client Connected");
      mqtt_publish(("Hi from " + device_id).c_str());
      mqtt_subscribe(topic_subscribe);
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(", retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String command;
  Serial.print("\n\nMessage arrived [");
  Serial.print(topic);
  Serial.println("] ");

  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  if (command.length() > 0) {
    Serial.println("Command received: " + command);
  }

  if (command.indexOf("LightOn") != -1) {
    lightStatus = 1;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("\nLED turned on by application");
    mqtt_publish(command.c_str());
  } else if (command.indexOf("LightOff") != -1) {
    lightStatus = 0;
    digitalWrite(LED_PIN, LOW);
    Serial.println("\nLED turned off by application");
    mqtt_publish(command.c_str());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(5000);
  Serial.println("\n\nWelcome to MQTT\n");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt_client.setCallback(callback);
  mqtt_connect();
}

void loop() {
  if (!mqtt_client.connected()) {
    mqtt_connect();
  }

  // Devices State Sync Request
  CurrentMillis = millis();

  if (CurrentMillis - PreviousMillis > DataSendingTime) {
    PreviousMillis = CurrentMillis;

    // Publish Light Data
    String pkt = "{\"device_id\":\"" + device_id + "\",\"type\":\"Light\",\"value\":" + String(lightStatus) + "}";
    mqtt_publish(pkt.c_str());
  }

  mqtt_client.loop();
}