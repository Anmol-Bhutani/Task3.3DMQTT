#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

/////// WiFi credentials
char ssid[] = "Miac's wifi";    // your network SSID
char pass[] = "12345678";    // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
int        port     = 1883;
const char topicWave[] = "SIT210/wave";
const char topicPat[]  = "SIT210/pat";

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(2, OUTPUT);  // For the LED
  while (!Serial);     // waits until the Serial Monitor opens

  // Connect to WiFi network
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // Connect to MQTT broker
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(5000);
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // Subscribe to both "wave" and "pat" topics
  mqttClient.subscribe(topicWave);
  mqttClient.subscribe(topicPat);

  Serial.println("Subscribed to topics:");
  Serial.println(topicWave);
  Serial.println(topicPat);
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // Received a message, print the topic and contents
    String receivedTopic = mqttClient.messageTopic();
    
    Serial.print("Received message on topic '");
    Serial.print(receivedTopic);
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();

    // Check which topic received the message
    if (receivedTopic.equals(topicWave)) {
      // Action for "wave" topic - flash LED 3 times
      flashLED(3);
    } else if (receivedTopic.equals(topicPat)) {
      // Action for "pat" topic - flash LED 5 times
      flashLED(5);
    }
  }
}

void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
  }
}