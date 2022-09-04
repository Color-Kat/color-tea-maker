#include <EncButton.h> // For buttons

/* --- PINS --- */
#define kettle_relay_pin 4   // Relay of kettle
#define termometr_pin A2     // Relay of kettle
#define hot_pump_pin 5       // Hot water
#define motor_pin_1 2        // Sugar dispenser
#define motor_pin_2 3        // Sugar dispenser
#define mixer_pin A1         // Mixer pin

#define l_button_pin 11      // Left button
#define r_button_pin 12      // Right button
#define potent_pin A3       // Potentiometer pin for set int values

/* --- Buttons --- */
EncButton<EB_TICK, l_button_pin> l_button; 
EncButton<EB_TICK, r_button_pin> r_button; 

#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h> // For lcd display
LCD_1602_RUS lcd(0x27, 16, 2);

void setup()
{
    Serial.begin(9600);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Чай машина OC");
    lcd.setCursor(4, 1);
    lcd.print("Loading...");

    r_button.setButtonLevel(HIGH);
    l_button.setButtonLevel(HIGH);

    pinMode(potent_pin, INPUT);
//    pinMode(mixer_pin, OUTPUT);
}

void loop() {
    l_button.tick();
    r_button.tick();
//    boolean isActive = digitalRead(sugar_button_pin);

    Serial.println(analogRead(potent_pin));
    delay(50);

    if(l_button.click()) {
        Serial.println("left");
    }
    
    if(r_button.click()) {
        Serial.println("right");
    }
}
