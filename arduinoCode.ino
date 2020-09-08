#include <SoftwareSerial.h>

SoftwareSerial esp(2, 3); // RX, TX

unsigned long lastBlink = 0;
unsigned long light_timer = 0;
unsigned long lamp_timer;
int light_level = 0;
String state;

void setup() {
  digitalWrite(LED_BUILTIN, HIGH);
  for(int i = 4; i < 13; i++){
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
  Serial.println("Serial Ready");
  esp.begin(9600);
  Serial.println("SoftSerial Ready");
  for(int i = 4; i < 13; i++){
    state = digitalRead(i) == HIGH ? " ON" : " OFF";
    esp.println(String(i) + state);
    Serial.println(String(i) + state);
    delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void loop (){
  if (esp.available()){
    int pin = esp.parseInt();
    if(pin < 13 && pin > 1){
      digitalWrite(pin, !digitalRead(pin));
      state = digitalRead(pin) == HIGH ? " ON" : " OFF";
      Serial.println(String(pin) + state);
      esp.println(String(pin) + state);
    }
  }
  // Blink the LED every 3 seconds
  if (millis() - lastBlink > 1500) {
    lastBlink = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  // Insert custon code below
  // For example, make synchronous calls to read a sensor

  // Photorresistor connected to pin A0 triggers a lamp controlled by pin A2
  light_level = analogRead(A0);
  if(light_level < 341 && digitalRead(A2) == LOW){
    lamp_timer = millis();
    digitalWrite(A2, HIGH);
    Serial.println("Lamp ON");
    esp.println("Lamp ON");
  }
  if((millis() - lamp_timer > 5000) && (digitalRead(A2) == HIGH) && (light_level > 680)){
    analogWrite(A2, 0);
    Serial.println("Lamp OFF");
    esp.println("Lamp OFF");
  }
  if (millis() - light_timer > 3000) {
    light_timer = millis();
    esp.println("Light " + String(light_level));
  }
}
