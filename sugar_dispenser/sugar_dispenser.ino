#define motor_pin_1 2
#define motor_pin_2 3

void setup() {
    Serial.begin(9600);
    pinMode(motor_pin_1, OUTPUT);
    pinMode(motor_pin_2, OUTPUT);
}
  
void loop() {
    digitalWrite(motor_pin_1, 0);
    digitalWrite(motor_pin_2, 1);
}
