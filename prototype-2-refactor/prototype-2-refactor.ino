#include <EncButton.h> // For buttons
#include <microDS18B20.h> // For thermometer DS18b20
#include <EEPROM.h> // For EEPROM memory to save settings
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
/* --- PINS --- */

#define INIT_ADDR 1023  // EEPROM addres for init key
#define INIT_KEY 1      // EEPROM init key

/* --- Initialize objects --- */
// Connect thermometer DS18b20
MicroDS18B20<termometr_pin> ds; 

// Buttons
EncButton<EB_TICK, l_button_pin> l_button; 
EncButton<EB_TICK, r_button_pin> r_button; 

// LCD Display
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x27, 16, 2);
Screen screen(&lcd); // Create screen object instance for convient work with lcd display
/* --- Initialize objects --- */

/* --- Initial values --- */
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

// Default settings
struct Settings{
    int cups_count_default = 1;
    int tea_temp_default = 75;
    int sugar_count_default = 2;
    int cup_pump_time = 11 * 1000;
    int sugar_spoon_time = 4 * 1000;
    int mixer_time = 5 * 1000;
} settings;

// Current values
int current_temp = 25; // Current temperature
int tea_temp = 75; // Desired temperature
int sugar_count = 2; // Number of spoons of sugar
int cups_count = 1; // Number of cups
/* --- Initial values --- */

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

    // Init EEPROM
    if (EEPROM.read(INIT_ADDR) != INIT_KEY) {
        // EEPROM haven't been initialized yet
        EEPROM.write(INIT_ADDR, INIT_KEY); // Write init key
        EEPROM.put(0, settings); // Set default data
    }

    // Read settings and apply it
    EEPROM.get(0, settings);
    tea_temp = settings.tea_temp_default;       
    sugar_count = settings.sugar_count_default; 
}

void loop() {
    l_button.tick();
    r_button.tick();

    screen.render();

    getTemperature(); // Temperature
    
    buttons();
}

void buttons() {
    static boolean isEdited = false;
    static unsigned long menu_delay_timer_1 = millis();
    static unsigned long menu_delay_timer_2 = millis();
  
    // Every second update info on LCD screen to display current temp
    static unsigned long updateTempTimer = millis();
    if(millis() - updateTempTimer > 1000) {
        screen.updateInfo();
        updateTempTimer = millis();
    }

    switch (currentMode) {
        // --- Normal Mode --- //
        case normalMode:
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
            if(l_button.hold() && r_button.hold() && millis() - menu_delay_timer_1 > 1000){
                isEdited = false;
                menu_delay_timer_2 = millis();
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
                lcd.setCursor(4, 1); // For blink
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
            screen.setMessage(sugar_count + String(" ч/л"));
            
            // Change temp by button
            if (l_button.click()) {
                isEdited = false;
                currentMode = normalMode;
                screen.update();
                break;
            }
          
            // Switch edit mode
            if (r_button.click())
                isEdited = !isEdited;

            if(isEdited){
                lcd.setCursor(5, 1); // For blink
                int newValue = potentRange(0, 4); // Get value from potentiometer

                // Update screen data
                if(newValue != sugar_count) {
                    sugar_count = newValue;
                    screen.update();
                }
            }
            
            break;
            
        // --- Settings Mode --- //
        case settingsMode:
            static const byte settingsCount = 5;
            static String settingsModeMessages[settingsCount] = {
                "->            OK",
                String("Кол-во ч/л: ") + settings.sugar_count_default,
                String("Температура: ") + settings.tea_temp_default,
                "Set sugar time",
                "Set pump time",
            };

            enum settingModes {
                start,
                sugar_count_default,
                tea_temp_default,
                set_sugar_time,
                set_pup_time
            };
            static settingModes currentSettingMode = start;
        
            screen.setHeader("Настройки");
            screen.setMessage(settingsModeMessages[currentSettingMode]);
            screen.setInfo("#", 15);

            // Go to the next settings menu item
            if(l_button.click()) {
                currentSettingMode = currentSettingMode == settingsCount-1 ? 0 : currentSettingMode + 1;
                isEdited = false;
                screen.update();
            }

            // Go to the edit mode
            if(r_button.click()) {
                isEdited = !isEdited;
                screen.setOverlap(isEdited);
                screen.update();

                if(!isEdited) EEPROM.put(0, settings);
            }

            // Change default sugar count
            if (currentSettingMode == sugar_count_default && isEdited) {
                lcd.setCursor(5, 1);
                int newValue = potentRange(0, 4); // Get value from potentiometer

                if(settings.sugar_count_default != newValue){
                    settings.sugar_count_default = newValue;
                    screen.setOverlapMessage(String(newValue) + String(" шт."));
                    screen.update();
                }
            }

            // Change default tea temperature
            if (currentSettingMode == tea_temp_default && isEdited) {
                lcd.setCursor(4, 1);
                int newValue = potentRange(20, 100); // Get value from potentiometer

                if(settings.tea_temp_default != newValue){
                    settings.tea_temp_default = newValue;
                    screen.setOverlapMessage(String(newValue) + String("°C"));
                    screen.update();
                }
            }

            // Go to the normal mode
            if(l_button.hold() && r_button.hold() && millis() - menu_delay_timer_2 > 1000){
                isEdited = false;
                menu_delay_timer_1 = millis();
                currentMode = normalMode; // Change mode
                screen.update();
                break;
            }
            
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
    return map(value, 0, 1024, min, max + 1);
}
