#include <microDS18B20.h> // For thermometer DS18b20
#include <GyverTM1637.h> // For led display
#include <EncButton.h> // For buttons
#include <EEPROM.h> // For EEPROM memory to save settings

#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
#include "Screen.h"

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

// --- Libs --- //

// Init buttons
EncButton<EB_TICK, temp_button_pin> temp_button;   
EncButton<EB_TICK, sugar_button_pin> sugar_button; 
EncButton<EB_TICK, start_button_pin> start_button; 

// Connect thermometer DS18b20
//MicroDS18B20<termometr_pin> ds; 

// Initialize Screen object
LCD_1602_RUS lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Screen screen(&lcd);

// --- Modes of tea machine --- //
enum modes {
    normalMode,    // Tea is not brewing, we can change params
    brewingMode,   // Tea is brewing
    settingsMode,  // Edit settings mode (change timers for components)
    boilingMode    // Boil the water
};
modes currentMode = normalMode;

int current_temp = 25; // Current temperature
int tea_temperature = 27; // Desired temperature
int sugar_count = 2; // Number of spoons of sugar
int cups_count = 1; // Number of cups

// Defaul settings
struct Settings{
    int cup_pump_time = 11 * 1000;
    int sugar_spoon_time = 4 * 1000;
    int mixer_time = 5 * 1000;
} settings;

#define INIT_ADDR 1023  // EEPROM addres for init key
#define INIT_KEY 0      // EEPROM init key

void setup()
{
    Serial.begin(9600);
//    screen.setMessage("Хай");
//    screen.render();
  
//    // Kettle
//    pinMode(kettle_relay_pin, OUTPUT);
//    digitalWrite(kettle_relay_pin, LOW);
//  
//    // Thermometer
//    pinMode(termometr_pin, INPUT);
//  
//    // Pump
//    pinMode(hot_pump_pin, OUTPUT);
//    digitalWrite(hot_pump_pin, LOW);
//  
//    // Mixer
//    pinMode(mixer_pin, OUTPUT);
//    digitalWrite(mixer_pin, LOW);
//  
//    // Sugar dispenser motor
//    pinMode(motor_pin_1, OUTPUT);
//    pinMode(motor_pin_2, OUTPUT);
//    digitalWrite(motor_pin_1, LOW);
//    digitalWrite(motor_pin_2, LOW);
//
//    // Init EEPROM
//    if (EEPROM.read(INIT_ADDR) != INIT_KEY) {
//        EEPROM.write(INIT_ADDR, INIT_KEY); // Write init key
//        
//        // Default data
//        EEPROM.put(0, settings);
//    }
//
//    // Read settings
//    EEPROM.get(0, settings);
//    Serial.println(settings.cup_pump_time);

    // Screen setup
//    lcd.init();
//    lcd.backlight();
//    lcd.setCursor(0, 0);
//    lcd.print("Тест");
//    lcd.setCursor(4, 1);
//    lcd.print("Loading...");
//    lcd.print("°C");

    
}

void loop()
{
    screen.render();
    Serial.println(111111111);
  
    temp_button.tick();
    sugar_button.tick();
    start_button.tick();
//  
//     buttons(); // Buttons actions
//  
//     getTemperature(); // Temperature=
    
     // Stages of making tea 
//     if(currentMode == brewingMode)
//        teaProcess();
     
//     screen.render(); // Display some information

//    screen.render();

    if (temp_button.click()) {
      Serial.println("temp");
        screen.setMessage("Привет)");
    }
    
     if (sugar_button.click()) {
      Serial.println("sugar");
        screen.setMessage("Привет2)");
    }
}

//void buttons() {
//    // Delay timers between switching menu modes
//    static unsigned long menu_delay_timer_1 = millis();
//    static unsigned long menu_delay_timer_2 = millis();
//  
//    switch (currentMode) {
//        // --- Normal Mode --- //
//        case normalMode: 
//           
//
//            // Go to the setting mode
//            if(millis() - menu_delay_timer_1 > 1000 && temp_button.hold() && sugar_button.hold()){
//                menu_delay_timer_2 = millis(); // Set delay between menu modes
//                currentMode = settingsMode;    // Change mode
//               
//                break;
//            }
//        
//            // Change temp by button
//            if (temp_button.click()) {
//                tea_temperature += 5;
//                
//                if(tea_temperature > 100) // Max temp is 100
//                    tea_temperature = 30;
//  
//                break;
//            }
//          
//            // Change number of sugar spoons by button
//            if (sugar_button.click()) {
//              sugar_count += 1;
//              if(sugar_count > 4) sugar_count = 0;
//              
//              break;
//            }
//
//            // Start boil water by hold Start button
//            if(start_button.hold()){
//                currentMode = boilingMode;
//                break;
//            }
//
//            // Start making tea by start button release
//            if(start_button.click()){
//                currentMode = brewingMode;
//                break;
//            }
//                        
//            break;
//
//       
//            
//        // --- Settings Mode --- //
//        case settingsMode:
//            static unsigned long press_button_delay = 0;
//            static unsigned long water_pump_start_time = 0;
//            static unsigned long sugar_dispenser_start_time = 0;
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
//                break;
//            }
//
//            
//            break; 
//
//        // --- Boiling Mode --- //
//        case boilingMode:
//            // Switch off the kettle when the temperature is riched
//            if(current_temp >= 95) {
//                digitalWrite(kettle_relay_pin, LOW); // Switch off the kettle
//                currentMode = normalMode;            // Change mode
//                break;
//            }
//
//            digitalWrite(kettle_relay_pin, HIGH);
//        
//            break;
//
//        // --- Brewing Mode --- //
//        case brewingMode:
//            break;
//    }
//}

/**
 * Every second get new value of the temperature
 */
void getTemperature(){
//    ds.requestTemp();
//    if(ds.readTemp()) current_temp = ds.getTemp();
//  
//    static unsigned long last_temp_request = 0;
//    if(millis() - last_temp_request > 1000) {
//        last_temp_request = millis();
//        if(ds.readTemp())
//            current_temp = ds.getTemp();
//      
//        ds.requestTemp();
//        Serial.println(current_temp);
//    }
}
