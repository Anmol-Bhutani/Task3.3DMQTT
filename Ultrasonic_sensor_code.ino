#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

#define EchoPin 2
#define TrigPin 3

/////// WiFi credentials
char ssid[] = "Miac's wifi";    // your network SSID (name)
char pass[] = "12345678";       // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
int        port     = 1883;
const char topicWave[] = "SIT210/wave";
const char topicPat[]  = "SIT210/pat";

long duration;
int distance;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Connect to WiFi
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
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // Keep MQTT connection alive
  mqttClient.poll();

  // Trigger the ultrasonic sensor
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  // Read the Echo pin
  duration = pulseIn(EchoPin, HIGH);
  
  // Calculate the distance (Duration / 2 because of round-trip)
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // Only publish if the distance is greater than 0
  if (distance > 0 && distance <= 20) {
    // Publish to "wave" topic
    Serial.print("Sending message to topic: ");
    Serial.println(topicWave);
    mqttClient.beginMessage(topicWave);
    mqttClient.print("Anmol Bhutani ");
    mqttClient.endMessage();

    // Publish to "pat" topic
    Serial.print("Sending message to topic: ");
    Serial.println(topicPat);
    mqttClient.beginMessage(topicPat);
    mqttClient.print("Anmol Bhutani - pat ");
    mqttClient.endMessage();

    Serial.println();
  }

  // Add a small delay to avoid flooding the MQTT broker
  delay(1000);
}