#define kettle_relay_pin A5   // Relay of kettle
#define hot_pump_pin A4       // Hot water 
#define motor_pin_1 2         // Sugar dispenser 
#define motor_pin_2 3         // Sugar dispenser 
#define mixer_relay_pin A1    // Hot water pump

#define switch_button_pin 12

int stage = 0;
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
        
        stage++;
        if(stage > 4) stage = 0;
        
        Serial.println(stage);
    }

    if(stage == 0) {
        Serial.println("Чайник включён");
        digitalWrite(kettle_relay_pin, 1);
    }

    if(stage == 1) {
        Serial.println("Помпа включена");
        digitalWrite(kettle_relay_pin, 0);
        
        digitalWrite(hot_pump_pin, 1);
    }

    if(stage == 2) {
        Serial.println("Дозатор сахара включён");
        digitalWrite(hot_pump_pin, 0);
        
        digitalWrite(motor_pin_1, 0);
        digitalWrite(motor_pin_2, 1);
    }

    if(stage == 3) {
        Serial.println("Включаем мешалку");

//        digitalWrite(motor_pin_1, 1);
//        digitalWrite(motor_pin_2, 1);
        
        digitalWrite(mixer_relay_pin, 1);
    }

    if(stage == 4) {
        digitalWrite(mixer_relay_pin, 0);
      
        Serial.println("Чай готов!");
    }
}
