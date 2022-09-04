#include <EncButton.h> // For buttons
#include <microDS18B20.h> // For thermometer DS18b20
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

// Connect thermometer DS18b20
MicroDS18B20<termometr_pin> ds; 

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
int tea_temp = 75; // Desired temperature
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
//    screen.update();
//    screen.updateInfo();
}

void loop() {
    l_button.tick();
    r_button.tick();

    screen.render();

    getTemperature(); // Temperature
    buttons();
//    Serial.println(potentRange(0, 4));
//    delay(50);
}

void buttons() {
    static boolean isEdited = false;
  
    // Every second update info on LCD screen to display current temp
    static unsigned long updateTempTimer = millis();
    if(millis() - updateTempTimer > 1000) {
        screen.updateInfo();
        updateTempTimer = millis();
    }

    switch (currentMode) {
        // --- Normal Mode --- //
        case normalMode:
//            screen.setHeader(String("     Чай: (") + current_temp + String("°C)"));
            screen.setHeader("     Чай:");
            screen.setInfo(String("(") + current_temp + String("°C)"), 10);
            screen.setMessage(
                tea_temp + String("°C  ") +
                sugar_count + String("Л   ") +
                cups_count + String("Круж")
            );

            // Update screen info for first painting
            static boolean isUpdated = false;
            if(!isUpdated){
                screen.update();
                isUpdated = true;
            }

            // Go to the setting mode
            if(l_button.hold() && r_button.hold()){
                Serial.println("hold");
                isEdited = false;
                currentMode = settingsMode; // Change mode
                screen.update();
                break;
            }
        
            // Go to the next menu item
            if (l_button.click()) {
                isEdited = false;
                currentMode = cupsMode;
                screen.update();
                break;
            }
                
            break;

        /* --- Set cups count --- */        
        case cupsMode:
            screen.setHeader("Кружки");
            screen.setInfo("", 15);
            screen.setMessage(String(cups_count) + String("шт."));

            // Change temp by button
            if (l_button.click()) {
                isEdited = false;
                currentMode = tempMode;
                screen.update();
                break;
            }

            // Switch edit mode
            if (r_button.click())
                isEdited = !isEdited;

            if(isEdited){
                lcd.setCursor(4, 1); // For blink
                int newValue = potentRange(1, 2); // Get value from potentiometer

                // Update screen data
                if(newValue != cups_count) {
                    cups_count = newValue;
                    screen.update();
                }
            }
            
            break;

        /* --- Set temperature --- */         
        case tempMode:
            screen.setHeader("Температура");
            screen.setInfo("", 15);
            screen.setMessage(String(tea_temp) + String("°C"));

            // Change temp by button
            if (l_button.click()) {
                isEdited = false;
                currentMode = sugarMode;
                screen.update();
                
                break;
            }
          
            // Switch edit mode
            if (r_button.click())
                isEdited = !isEdited;

            if(isEdited){
                lcd.setCursor(5, 1); // For blink
                int newValue = potentRange(20, 100); // Get value from potentiometer

                // Update screen data
                if(newValue != tea_temp) {
                    tea_temp = newValue;
                    screen.update();
                }
            }
            
            break;

       /* --- Set spoons count --- */   
       case sugarMode:
            screen.setHeader("Сахар");
            screen.setInfo("", 15);
            screen.setMessage("2 ложки");
            
            // Change temp by button
            if (l_button.click()) {
                isEdited = false;
                currentMode = normalMode;
                screen.update();
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
            screen.setMessage("Дозатор сахара");
            screen.setInfo("#", 15);

            // Go to the normal mode
            if(l_button.hold() && r_button.hold()){
                isEdited = false;
                currentMode = normalMode; // Change mode
                screen.update();
                break;
            }
            
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

    if(isEdited) screen.blink();
    else screen.noBlink();
}

/**
 * Every second get new value of the temperature
 */
void getTemperature(){
    ds.requestTemp();
    if(ds.readTemp()) current_temp = ds.getTemp();
  
    static unsigned long last_temp_request = 0;
    if(millis() - last_temp_request > 1000) {
        last_temp_request = millis();
        if(ds.readTemp())
            current_temp = ds.getTemp();
      
        ds.requestTemp();
        Serial.println(current_temp);
    }
}

int potentRange(int min, int max) {
    int value = analogRead(potent_pin);
    Serial.println(value);
    return map(value, 0, 1024, min, max + 1);
    
    value = map(value, 0, 1024, min, max + 1);
    value = constrain(value, min, max);
    return value;
}
