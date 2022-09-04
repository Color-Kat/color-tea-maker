#include <EncButton.h> // For buttons

/* --- PINS --- */
#define kettle_relay_pin 4   // Relay of kettle
#define termometr_pin A2     // Relay of kettle
#define hot_pump_pin 5       // Hot water
#define motor_pin_1 2        // Sugar dispenser
#define motor_pin_2 3        // Sugar dispenser
#define mixer_pin A1         // Mixer pin

#define temp_button_pin 12   // Button for change the temperature
#define sugar_button_pin 11  // Button for change number of sugar spoons
#define start_button_pin 10  // Button for start brewing tea

/* --- Buttons --- */
EncButton<EB_TICK, sugar_button_pin> sugar_button; 

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

//    pinMode(sugar_button_pin, INPUT);
    sugar_button.setButtonLevel(HIGH);
    pinMode(mixer_pin, OUTPUT);
}

void loop() {
    sugar_button.tick();
//    boolean isActive = digitalRead(sugar_button_pin);

    if(sugar_button.hold()) {
      Serial.println("click");
        digitalWrite(mixer_pin, HIGH);
    } else {
        digitalWrite(mixer_pin, LOW);
    }
}
