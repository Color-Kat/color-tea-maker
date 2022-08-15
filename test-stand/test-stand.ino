#include <microDS18B20.h> // For thermometer DS18b20
#include <GyverTM1637.h> // For led display
#include <EncButton.h> // For buttons
#include <EEPROM.h> // For EEPROM memory to save settings
#include "Screen.h"

/* --- PINS --- */
#define kettle_relay_pin 4   // Relay of kettle
#define termometr_pin A2     // Relay of kettle
#define hot_pump_pin 5       // Hot water
#define motor_pin_1 2        // Sugar dispenser
#define motor_pin_2 3        // Sugar dispenser
#define mixer_relay_pin A1   // Mixer pin
#define display_CLK A3       // CLK pin of led display
#define display_DIO A4       // CLK pin of led display

#define temp_button_pin 12   // Button for change the temperature
#define sugar_button_pin 11  // Button for change number of sugar spoons
#define start_button_pin 10  // Button for start brewing tea

// --- Libs --- //

// Init buttons
EncButton<EB_TICK, temp_button_pin> temp_button;   
EncButton<EB_TICK, sugar_button_pin> sugar_button; 
EncButton<EB_TICK, start_button_pin> start_button; 

// Connect thermometer DS18b20
MicroDS18B20<termometr_pin> ds; 

// Connect led display
GyverTM1637 disp(display_CLK, display_DIO);

// Initialize Screen object
Screen screen(&disp);

// --- Modes of tea machine --- //
enum modes {
    normalMode, // Tea is not brewing, we can change params
    brewingMode, // Tea is brewing
    settingsMode, // Edit settings mode (change timers for components)
};
modes currentMode = normalMode;

int current_temp = 30; // Current temperature
int tea_temperature = 20; // Desired temperature
int sugar_count = 2; // Number of spoons of sugar
int cups_count = 0; // Number of cups

/* --- Timers --- */
int cup_pump_time = 11 * 1000;
int sugar_spoon_time = 4 * 1000;
int mixer_time = 20 * 1000;

// Defaul settings
struct Settings{
    int cup_pump_time = 11 * 1000;
    int sugar_spoon_time = 4 * 1000;
    int mixer_time = 5 * 1000;
} settings;

#define INIT_ADDR 1023  // EEPROM addres for init key
#define INIT_KEY 0     // EEPROM init key

void setup()
{
    Serial.begin(9600);
  
    // Kettle
    pinMode(kettle_relay_pin, OUTPUT);
    digitalWrite(kettle_relay_pin, LOW);
  
    // Thermometer
    pinMode(termometr_pin, INPUT);
  
    // Pump
    pinMode(hot_pump_pin, OUTPUT);
    digitalWrite(hot_pump_pin, LOW);
  
    // Mixer
    pinMode(mixer_relay_pin, OUTPUT);
    digitalWrite(mixer_relay_pin, LOW);
  
    // Sugar dispenser motor
    pinMode(motor_pin_1, OUTPUT);
    pinMode(motor_pin_2, OUTPUT);
    digitalWrite(motor_pin_1, LOW);
    digitalWrite(motor_pin_2, LOW);
  
    screen.updateState(); // First update for display menu header
    disp.displayByte(_t, _e, _a, _empty);

    // Init EEPROM
    if (EEPROM.read(INIT_ADDR) != INIT_KEY) {
        EEPROM.write(INIT_ADDR, INIT_KEY); // Write init key
        
        // Default data
        EEPROM.put(0, settings);
    }

    // Read settings
    EEPROM.get(0, settings);
    Serial.println(settings.cup_pump_time);
}

void loop()
{
     temp_button.tick();
     sugar_button.tick();
     start_button.tick();
  
     buttons(); // Buttons actions
  
     getTemperature(); // Temperature=
    
     // Stages of making tea 
     if(currentMode == brewingMode)
        teaProcess();
     
     screen.render(); // Display some information
}

void buttons() {
    // Delay timers between switching menu modes
    static unsigned long menu_delay_timer_1 = millis();
    static unsigned long menu_delay_timer_2 = millis();
  
    switch (currentMode) {
        // --- Normal Mode --- //
        case normalMode: 
            screen.setHeader(_S, _t, _O, _P); // Menu header
            screen.setMessage(current_temp, _t); // Menu main text is temperature

            // Go to the setting mode
            if(millis() - menu_delay_timer_1 > 1000 && temp_button.hold() && sugar_button.hold()){
                menu_delay_timer_2 = millis(); // Set delay between menu modes
                currentMode = settingsMode;    // Change mode
                screen.updateState();          // Update header on display
                
                break;
            }
        
            // Change temp by button
            if (temp_button.click()) {
                tea_temperature += 5;
                
                if(tea_temperature > 100) // Max temp is 100
                    tea_temperature = 30;

                // Show temp on display
                screen.setNumber(tea_temperature, _t);
  
                break;
            }
          
            // Change number of sugar spoons by button
            if (sugar_button.click()) {
              sugar_count += 1;
              if(sugar_count > 4) sugar_count = 0;

              // Show count on display
              screen.setNumber(sugar_count, _c);
              
              break;
            }

            // Start making tea by start button release
            if(start_button.click()){
                screen.updateState();
                currentMode = brewingMode;
            }
                        
            break;

        // --- Brewing Mode --- //
        case brewingMode:
            break;
            
        // --- Settings Mode --- //
        case settingsMode:
            static unsigned long press_button_delay = 0;
            static unsigned long water_pump_start_time = 0;
            static unsigned long sugar_dispenser_start_time = 0;
            
            screen.setHeader(_S, _e, _t, _empty);
            screen.setMessage(_C, _empty, _empty, _8);

            // Add some delay
            if(millis() - menu_delay_timer_2 <= 1000) {
                water_pump_start_time = millis();
                sugar_dispenser_start_time = millis();
                press_button_delay = 0;
                break; 
            }

            // Go to the normal mode
            if(temp_button.hold() && sugar_button.hold()){
                menu_delay_timer_1 = millis();  // Set delay between menu modes
                currentMode = normalMode;       // Change mode
                screen.updateState();           // Update header on display
                break;
            }

            changeWaterPumpTime(water_pump_start_time);
            changeSugarDispenserTime(sugar_dispenser_start_time);
            
            break; 
    }
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

/**
 * Control the stages of making tea:
 * - Kettle
 * - Water pump
 * - Sugar dispenser
 * - Mixer
 */
void teaProcess(){
    static uint8_t stage = 0;
    static long timer = millis();

    // Stop making tea by start button click
    if(start_button.click()){
        stage = 4;
        timer = millis();
        digitalWrite(kettle_relay_pin, LOW);
    }
    
    switch (stage) {
      // Kettle
      case 0:
        Serial.println("Чайник включён");
        digitalWrite(kettle_relay_pin, HIGH);

        screen.setHeader(_9, _O, _empty, _empty);
        screen.setMessage(current_temp, _t);

        if(current_temp >= tea_temperature){
          stage++;
          timer = millis();
          digitalWrite(kettle_relay_pin, LOW);
        }
        
        break;

      // Pump
      case 1:
        Serial.println("Помпа включена");

        screen.setMessage(_P, _U, 0x40, _P);

        digitalWrite(hot_pump_pin, HIGH);

        if(millis() - timer > settings.cup_pump_time * cups_count) {
            stage++;
            timer = millis();
            digitalWrite(hot_pump_pin, LOW);
        }
        
        break;

      // Sugar
      case 2: {
          Serial.println("Дозатор сахара включён");
  
          // Skip stage if there is no sugar
          if(sugar_count == 0) {
              stage++;
              timer = millis();
              break;
          }

          unsigned long totalTime = settings.sugar_spoon_time * sugar_count;
          
          screen.setMessage(int((float)(millis() - timer + 300) / totalTime * sugar_count) , _c);
  
          // Turn on the sugar dispenser
          digitalWrite(motor_pin_1, LOW);
          digitalWrite(motor_pin_2, HIGH);
  
          // Go to the next stage after time
          if(millis() - timer > totalTime) {
              stage++;
              timer = millis();
              digitalWrite(motor_pin_1, HIGH);
              digitalWrite(motor_pin_2, HIGH);
          }
        }
        
        break;

      // Mixer
      case 3:
        Serial.println("Включаем мешалку");

        screen.setMessage(_S, _H, _U, _F);

        digitalWrite(mixer_relay_pin, HIGH);

        if(millis() - timer > mixer_time) {
            stage++;
            timer = millis();
            digitalWrite(mixer_relay_pin, LOW);
        }
        
        break;

      // Done
      case 4:
        Serial.println("Чай готов!");
        screen.setInfo(_empty, _E, _n, _d);
        currentMode = normalMode;
        stage = 0;
        break;

      default:
        break;
    }
}

/**
 * Change one cup water pump time by temp_button
 */
void changeWaterPumpTime(unsigned long _timer) {
    static unsigned long timer = _timer; // Copy timer with default value
    
    if(temp_button.press()) {
        // Show prevous value on display
        screen.setNumber(settings.cup_pump_time / 1000, _c);

        timer = millis(); // Save start time
    }
  
    // Display new value when hold the button
    if(temp_button.hold()) {
        // Get seconds with 1 sec delay of hold
        int seconds = (millis() - timer - 1000) / 1000;

        // Start water pump
        digitalWrite(hot_pump_pin, HIGH);

        // Display new value
        screen.setNumber(seconds, _c);
    }
  
    // Write he new value to EEPROM when the button is released
    if(temp_button.release()) {
        // Get time with 1 sec delay of hold
        long water_pump_time = millis() - timer - 1000; // 

        // Stop water pump
        digitalWrite(hot_pump_pin, LOW);
      
        if(timer > 0 && water_pump_time > 1000){
            settings.cup_pump_time = water_pump_time;
            screen.setInfo(_S, _A, _U, _E);
        
            EEPROM.put(0, settings); // Save to EEPROM
        }
    }
}

/**
 * Change sugar dispenser one spoon time by sugar_button
 */
void changeSugarDispenserTime(unsigned long _timer) {
    static unsigned long timer = _timer; // Copy timer with default value
    
    if(sugar_button.press()) {
        // Show prevous value on display
        screen.setNumber(settings.sugar_spoon_time / 1000, _c);

        timer = millis(); // Save start time
    }
  
    // Display new value when hold the button
    if(sugar_button.hold()) {
        // Get seconds with 1 sec delay of hold
        int seconds = (millis() - timer - 1000) / 1000;

        // Start sugar dipenser
        digitalWrite(motor_pin_1, LOW);
        digitalWrite(motor_pin_2, HIGH);

        // Display new value
        screen.setNumber(seconds, _c);
    }
  
    // Write he new value to EEPROM when the button is released
    if(sugar_button.release()) {
        // Get time with 1 sec delay of hold
        long water_pump_time = millis() - timer - 1000; // 

        // Stop sugar dispenser
        digitalWrite(motor_pin_1, HIGH);
        digitalWrite(motor_pin_2, HIGH);
      
        if(timer > 0 && water_pump_time > 1000){
            settings.sugar_spoon_time = water_pump_time;
            screen.setInfo(_S, _A, _U, _E);
        
            EEPROM.put(0, settings); // Save to EEPROM
        }
    }
}
