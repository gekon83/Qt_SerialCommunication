const uint8_t ledPin = LED_BUILTIN;
uint32_t interval = 1000;
uint32_t previousMillis = 0;
 
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}
 
void loop() {
  uint32_t currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    Serial.println("Processor time = " + String(currentMillis));
  }
}
 
void serialEvent() {
  while (Serial.available()) {
    char state = (char)Serial.read();
    if(state == '0') {
      digitalWrite(ledPin, 0);
    } else if(state == '1') {
      digitalWrite(ledPin, 1);
    }
 
    Serial.println("Received. LED status = " + String(state));
  }
}
