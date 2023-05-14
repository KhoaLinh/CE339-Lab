#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "Khoa Linh";
const char* password = "khoalinh1134";

const char* mqtt_server = "55ec43b0dfd5490fa76b27cbb22bc247.s2.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "khoalinh";
const char* mqtt_password = "Khoalinh123";

const char* topic = "LedTest";
const int LED_PIN = 2; // Pin number of the LED

WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Received message: " + message);

  if (message == "LightOn") {
    digitalWrite(LED_PIN, HIGH); // Turn on LED
    Serial.println("LED turned on");
  } else if (message == "LightOff") {
    digitalWrite(LED_PIN, LOW); // Turn off LED
    Serial.println("LED turned off");
  } else {
    Serial.println("Invalid message");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT); // Set LED pin as output

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT broker
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32_client", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(topic);
    } else {
      Serial.print("MQTT connection failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  client.loop();
}