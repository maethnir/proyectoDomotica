#include <SoftwareSerial.h>

SoftwareSerial esp(2, 3); // RX, TX

unsigned long lastBlink = 0;
String state;

void setup() {
  for(int i = 4; i < 13; i++){
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
  esp.begin(9600);
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
}
