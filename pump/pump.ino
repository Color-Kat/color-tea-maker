#define button_pin 2
#define mosfet_pin 13

boolean pump = 0; // State of the relay

unsigned long last_click = 0;

void setup() {
    Serial.begin(9600);
    pinMode(button_pin, INPUT_PULLUP);
    pinMode(mosfet_pin, OUTPUT);
}
  
void loop() {
    boolean button = !digitalRead(button_pin);

    if(button == 1 && millis() - last_click > 200){
        last_click = millis();
      
        pump = !pump;
    }

    Serial.println(pump);

    digitalWrite(mosfet_pin, pump);
}
