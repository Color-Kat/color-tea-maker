#define kettle_relay_pin A3   // Relay of kettle
#define hot_pump_pin A4       // Hot water 
#define mixer_relay_pin A1    // Hot water pump
#define motor_pin_1 2         // Sugar dispenser 
#define motor_pin_2 3         // Sugar dispenser 

#define switch_button_pin 12

int state = 0;
unsigned long last_click = 0;

void setup() {
    Serial.begin(9600);

    pinMode(kettle_relay_pin, OUTPUT);
    pinMode(hot_pump_pin, OUTPUT);
    pinMode(mixer_relay_pin, OUTPUT);
    pinMode(motor_pin_1, OUTPUT);
    pinMode(motor_pin_2, OUTPUT);

    pinMode(switch_button_pin, INPUT_PULLUP);
}
  
void loop() {
//    digitalWrite(motor_pin_1, 0);
//    digitalWrite(motor_pin_2, 1);

    boolean button = !digitalRead(switch_button_pin);

    // Switch state of making tea
    if(button == 1 && millis() - last_click > 300){
        last_click = millis();
        
        state++;
        if(state > 4) state = 0;
        
        Serial.println(state);
    }

    if(state == 0) {
        Serial.println("Чайник включён");
    }

    if(state == 1) {
        Serial.println("Помпа включена");
    }

    if(state == 2) {
        Serial.println("Дозатор сахара включён");
    }

    if(state == 3) {
        Serial.println("Включаем мешалку");
    }

    if(state == 4) {
        Serial.println("Чай готов!");
    }
}
