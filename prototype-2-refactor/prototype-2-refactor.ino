#include <EncButton.h> // For buttons
#include "Screen.h"

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

Screen screen(&lcd);

// --- Modes of tea machine --- //
enum modes {
    normalMode,
    tempMode,    // Tea is not brewing, we can change params
    sugarMode,
    cupsMode,
    settingsMode,  // Edit settings mode (change timers for components)
    brewingMode,   // Tea is brewing
    boilingMode    // Boil the water
};
modes currentMode = normalMode;

// Current values
int current_temp = 25; // Current temperature
int tea_temp = 27; // Desired temperature
int sugar_count = 2; // Number of spoons of sugar
int cups_count = 1; // Number of cups

void setup()
{
    Serial.begin(9600);

    r_button.setButtonLevel(HIGH);
    l_button.setButtonLevel(HIGH);

    // Thermometer
    pinMode(termometr_pin, INPUT);

    pinMode(potent_pin, INPUT);
//    pinMode(mixer_pin, OUTPUT);

    screen.init();
    screen.update();
}

void loop() {
    l_button.tick();
    r_button.tick();

    screen.render();

    buttons();
//    Serial.println(potentRange(0, 4));
//    delay(50);
}

void buttons() {

    Serial.println(currentMode);

    switch (currentMode) {
        // --- Normal Mode --- //
        case normalMode:
            screen.setHeader(String("     Чай: (") + current_temp + String("°C)"));
            screen.setMessage(
              tea_temp + String("°C  ") +
              sugar_count + String("Л   ") +
              cups_count + String("Круж")
            );

            // Go to the setting mode
            if(l_button.hold() && r_button.hold()){
                screen.update();
                currentMode = settingsMode; // Change mode
                break;
            }
        
            // Change temp by button
            if (l_button.click()) {
                screen.update();
                currentMode = cupsMode;
                break;
            }
                
            break;

        case cupsMode:
            screen.setHeader("Кружки");
            screen.setMessage("2");

            // Change temp by button
            if (l_button.click()) {
                screen.update();
                currentMode = tempMode;
                break;
            }
          
            // Change number of sugar spoons by button
            if (r_button.click()) {
                
                break;
            }
            
            break;

        case tempMode:
            screen.setHeader("Температура");
            screen.setMessage("25");

            // Change temp by button
            if (l_button.click()) {
                screen.update();
                currentMode = sugarMode;
                
                break;
            }
          
            // Change number of sugar spoons by button
            if (r_button.click()) {
                
                break;
            }
            
            break;

       case sugarMode:
            screen.setHeader("Сахар");
            screen.setMessage("2 ложки");

            // Change temp by button
            if (l_button.click()) {
                screen.update();
                currentMode = normalMode;
                break;
            }
          
            // Change number of sugar spoons by button
            if (r_button.click()) {
                
                break;
            }
            
            break;
            
        // --- Settings Mode --- //
        case settingsMode:
            screen.setHeader("Настройки");
            screen.setMessage("2 ложки");
//            static unsigned long press_button_delay = 0;
//            static unsigned long water_pump_start_time = 0;
//            static unsigned long sugar_dispenser_start_time = 0;
//            
//            screen.setHeader(_S, _e, _t, _empty);
//            screen.setMessage(_C, _empty, _empty, _8);
//
//            // Add some delay
//            if(millis() - menu_delay_timer_2 <= 1000) {
//                water_pump_start_time = millis();
//                sugar_dispenser_start_time = millis();
//                press_button_delay = 0;
//                break; 
//            }
//
//            // Go to the normal mode
//            if(temp_button.hold() && sugar_button.hold()){
//                menu_delay_timer_1 = millis();  // Set delay between menu modes
//                currentMode = normalMode;       // Change mode
//                screen.updateState();           // Update header on display
//                break;
//            }
//
//            changeWaterPumpTime(water_pump_start_time);
//            changeSugarDispenserTime(sugar_dispenser_start_time);
            
            break; 

        // --- Boiling Mode --- //
        case boilingMode:
           

            // Switch off the kettle when the temperature is riched
//            if(current_temp >= 95) {
//                digitalWrite(kettle_relay_pin, LOW); // Switch off the kettle
//                currentMode = normalMode;            // Change mode
//                screen.updateState();                // Update header on display
//                break;
//            }
//
//            digitalWrite(kettle_relay_pin, HIGH);
        
            break;

        // --- Brewing Mode --- //
        case brewingMode:
            break;
    }
}

int potentRange(int min, int max) {
    int value = analogRead(potent_pin);
    value = map(value, 0, 1023, min, max);
    value = constrain(value, min, max);
    return value;
}
