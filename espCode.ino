#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const bool DEBUG = false;

// Wifi configuration
const char* ssid = "RaspiHouse";
const char* password = "ValidateMyIoT";
const char* mqtt_server = "192.168.4.1";

// MQTT Configuration
const String clientId = "ESP2";
const char* inTopic = "raspihouse/espin/2";
const char* outTopic = "raspihouse/espout/2";

// Client Objects
WiFiClient espClient;
PubSubClient client(espClient);

//Message buffers
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if(DEBUG){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
  }
  payload[length] = '\0';
  Serial.println((char*)payload);
  payload = '\0';
  /*for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
  }*/
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      client.publish(outTopic, "Connected");
      if(DEBUG){
        Serial.println("Connected");
      }
      client.subscribe(inTopic);
    }
    else {
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if(DEBUG){
    Serial.print("Setup coplete");
  }
}

unsigned long lastBlink = millis();
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available()){
    String str = Serial.readString();
    int len = str.length();
    char payload[len];
    str.toCharArray(payload, len);
    client.publish(outTopic, payload);
  }

  // Blink the LED every 3 seconds
  if (millis() - lastBlink > 1500) {
    lastBlink = millis();
    if(DEBUG){
      Serial.print("...");
    }
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
