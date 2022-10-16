//#include <EncButton.h>     // For buttons
#include <microDS18B20.h>  // For thermometer DS18b20
//#include <EEPROM.h>        // For EEPROM memory to save settings
//#include <ezBuzzer.h>      // For buzzer
//#include "Screen.h"

/* --- PINS --- */
//#define kettle_relay_pin 4     // Relay of kettle
//#define mixer_pin 5            // Mixer pin
#define termometer_pin A2      // Termometer pin
//#define hot_pump_pin A0        // Hot water
//#define sugar_dispenser_pin A1 // Sugar dispenser
//#define buzzer_pin 9           // Buzzer pin

//#define l_button_pin 11      // Left button
//#define r_button_pin 12      // Right button
//#define potent_pin A3        // Potentiometer pin for set int values
/* --- PINS --- */

#define INIT_ADDR 1023  // EEPROM addres for init key
#define INIT_KEY 1      // EEPROM init key

/* --- Initialize objects --- */
// Connect thermometer DS18b20
MicroDS18B20<termometer_pin> ds; 

//ezBuzzer buzzer(buzzer_pin); // create ezBuzzer object that attach to a pin;
//
//// Buttons
//EncButton<EB_TICK, l_button_pin> l_button; 
//EncButton<EB_TICK, r_button_pin> r_button; 
//
//// LCD Display
//#define _LCD_TYPE 1
//#include <LCD_1602_RUS_ALL.h>
//LCD_1602_RUS lcd(0x27, 16, 2);
//Screen screen(&lcd); // Create screen object instance for convient work with lcd display
///* --- Initialize objects --- */
//
///* --- Initial values --- */
//// --- Modes of tea machine --- //
//enum modes {
//    normalMode,
//    tempMode,    // Tea is not brewing, we can change params
//    sugarMode,
//    cupsMode,
//    settingsMode,  // Edit settings mode (change timers for components)
//    brewingMode,   // Tea is brewing
//    boilingMode    // Boil the water
//};
//modes currentMode = normalMode;
//
//// Default settings
//struct Settings{
//    int cups_count_default = 1;
//    int tea_temp_default = 75;
//    int sugar_count_default = 2;
//    int cup_pump_time = 11 * 1000;
//    int sugar_spoon_time = 4 * 1000;
//    int mixer_time = 15 * 1000;
//} settings;
//
//// Current values
//int current_temp = 25; // Current temperature
//int tea_temp = 75; // Desired temperature
//int sugar_count = 2; // Number of spoons of sugar
//int cups_count = 1; // Number of cups
///* --- Initial values --- */

void setup()
{
    Serial.begin(9600);

    // Buttons and potentiometer
//    r_button.setButtonLevel(HIGH);
//    l_button.setButtonLevel(HIGH);
//    pinMode(potent_pin, INPUT);

    // Thermometer
//    pinMode(termometer_pin, INPUT);

    // Kettle
//    pinMode(kettle_relay_pin, OUTPUT);
//    digitalWrite(kettle_relay_pin, LOW);
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
//    pinMode(sugar_dispenser_pin, OUTPUT);
//    digitalWrite(sugar_dispenser_pin, LOW);
//
//    // Buzzer
//    pinMode(buzzer_pin, OUTPUT);

//    // Init EEPROM
//    if (EEPROM.read(INIT_ADDR) != INIT_KEY) {
//        // EEPROM haven't been initialized yet
//        EEPROM.write(INIT_ADDR, INIT_KEY); // Write init key
//        EEPROM.put(0, settings); // Set default data
//    }
//
//    // Read settings and apply it
//    EEPROM.get(0, settings);
//    tea_temp = settings.tea_temp_default;       
//    sugar_count = settings.sugar_count_default; 
//    settings.mixer_time = 15 * 1000;

    // Init display
//    screen.init();
}

//unsigned long screenUpdateTimer = 0;

void loop() {
//    buzzer.loop();
//  
//    l_button.tick();
//    r_button.tick();
//
//    screen.render(); // Always render information on display

    getTemperature(); // Temperature

//    Serial.println(ds.online());
  
//    ds.requestTemp();
//    
//    // вместо delay используй таймер на millis(), пример async_read
//    delay(1000);
//    
//    // проверяем успешность чтения и выводим
//    if (ds.readTemp()) Serial.println(ds.getTemp());
//    else Serial.println("error");
    
//    menuControl();
//
//    // Stages of making tea 
//    if(currentMode == brewingMode)
//        teaProcess();
//
//    // Boil new raw water
//    if(currentMode == boilingMode)
//        boil();
}

/**
 * Work with buttons and menu navigation
 */
//void menuControl() {
//    static boolean isEdited = false; // If true, it's edit mode
//    static unsigned long menu_delay_timer_1 = millis(); // Delay when change settings and normal mode
//    static unsigned long menu_delay_timer_2 = millis(); // Delay when change settings and normal mode
//  
//    // Every second update info on LCD screen to display current temp
//    static unsigned long updateTempTimer = millis();
//    if(millis() - updateTempTimer > 1000) {
//        screen.updateInfo();
//        updateTempTimer = millis();
//    }
//
//    switch (currentMode) {
//        // --- Normal Mode --- //
//        case normalMode:
//            // Main screen with main params of tea
//            screen.setHeader("     Чай:");
//            screen.setInfo(String("(") + current_temp + String("°C)"), 10); // Display (current temp)
//            screen.setMessage(
//                tea_temp + String("°C  ") +
//                sugar_count + String("Л   ") +
//                cups_count + String("Круж")
//            );
//
//            // Update screen info for first painting
//            static boolean isUpdated = false;
//            if(!isUpdated){
//                screen.update();
//                isUpdated = true;
//            }
//
//            // Go to the setting mode
//            if(l_button.hold() && r_button.hold() && millis() - menu_delay_timer_1 > 900){
//                isEdited = false;              // Switch off edit mode
//                menu_delay_timer_2 = millis(); // Set delay from start change menu mode
//                currentMode = settingsMode;    // Change mode
//                screen.update();
//                changeSound();
//                break;
//            }
//
//            // Start making tea
//            if(r_button.hold() && !l_button.state() && millis() - menu_delay_timer_1 > 1100) {
//                currentMode = brewingMode;
//                screen.update();
//                changeSound();
//                break;
//            }
//
//            // Start boil water
//            if(l_button.hold() && !r_button.state() && millis() - menu_delay_timer_1 > 1100) {
//                currentMode = boilingMode;
//                screen.update();
//                changeSound();
//                break;
//            }
//        
//            // Go to the next menu item
//            if (l_button.click()) {
//                isEdited = false;       // Switch off edit mode
//                currentMode = cupsMode; // Next mode is cups mode settings
//                screen.update();        // Update info in advance
//                clickSound();
//                break;
//            }
//                
//            break;
//
//        // --- Set cups count --- //   
//        case cupsMode:
//            // Display cups mode menu text
//            screen.setHeader("Кружки");
//            screen.setInfo("", 15);                                 // Clear info message
//            screen.setMessage(String(cups_count) + String("шт."));  // Display count of cups
//
//            // Go to the change tea temperature menu page
//            if (l_button.click()) {
//                isEdited = false; // Switch off edit mode
//                currentMode = tempMode;
//                screen.update();
//                clickSound();
//                break;
//            }
//
//            // Switch edit mode
//            if (r_button.click()) {
//                isEdited = !isEdited;
//                editSound();
//            }
//
//            // Update data (cups count) on lcd in edit mode
//            if(isEdited){
//                lcd.setCursor(4, 1);              // For blink
//                int newValue = potentRange(1, 2); // Get value from potentiometer
//
//                // Update screen data
//                if(newValue != cups_count) {
//                    cups_count = newValue;
//                    screen.update();
//                }
//            }
//            
//            break;
//
//        // --- Set tea temperature--- //     
//        case tempMode:
//            // Main tea temp menu page text
//            screen.setHeader("Температура");
//            screen.setInfo("", 15);                             // Clear info message
//            screen.setMessage(String(tea_temp) + String("°C")); // Current tea temp
//
//            // Change temp by button
//            if (l_button.click()) {
//                isEdited = false; // Switch off edit mode
//                currentMode = sugarMode;
//                screen.update();
//                clickSound();
//                
//                break;
//            }
//          
//             // Switch edit mode
//            if (r_button.click()) {
//                isEdited = !isEdited;
//                editSound();
//            }
//
//            // Update tea temp on lcd display in edit mode
//            if(isEdited){
//                lcd.setCursor(4, 1);                 // For blink
//                int newValue = potentRange(20, 100); // Get value from potentiometer
//
//                // Update screen data
//                if(newValue != tea_temp) {
//                    tea_temp = newValue;
//                    screen.update();
//                }
//            }
//            
//            break;
//
//       // --- Set spoons count --- //
//       case sugarMode:
//            // Spoons count menu page main text
//            screen.setHeader("Сахар");
//            screen.setInfo("", 15);                          // Clear info message
//            screen.setMessage(sugar_count + String(" ч/л")); // Dispaly current count of sugar
//            
//            // Change temp by button
//            if (l_button.click()) {
//                isEdited = false; // Switch off edit mode
//                currentMode = normalMode;
//                screen.update();
//                clickSound();
//                
//                break;
//            }
//          
//            // Switch edit mode
//            if (r_button.click()) {
//                isEdited = !isEdited;
//                editSound();
//            }
//
//            // Update and display on lcd screen new sugar spoons count
//            if(isEdited){
//                lcd.setCursor(5, 1);              // For blink
//                int newValue = potentRange(0, 4); // Get value from potentiometer
//
//                // Update screen data
//                if(newValue != sugar_count) {
//                    sugar_count = newValue;
//                    screen.update();
//                }
//            }
//            
//            break;
//            
//        // --- Settings Mode --- //
//        case settingsMode:
//            // It's array of settings menu pages headers
//            static const byte settingsCount = 5;
//            static String settingsModeMessages[settingsCount] = {
//                "->            OK",
//                String("Кол-во ч/л: ") + settings.sugar_count_default,
//                String("Температура: ") + settings.tea_temp_default,
//                "Set sugar time",
//                "Set pump time",
//            };
//
//            // Enum of settings menu pages (modes)
//            enum settingModes {
//                start,
//                sugar_count_default,
//                tea_temp_default,
//                set_sugar_time,
//                set_pump_time
//            };
//            // Current setting page
//            static settingModes currentSettingMode = start;
//
//            // Screen main screen text
//            screen.setHeader("Настройки");
//            screen.setMessage(settingsModeMessages[currentSettingMode]);
//            screen.setInfo("#", 15);
//
//            // Go to the next settings menu item
//            if(l_button.click()) {
//                currentSettingMode = currentSettingMode == settingsCount-1 ? 0 : currentSettingMode + 1;
//                isEdited = false; // Switch off edit mode
//                screen.setOverlap(false); // Overlap is used for display new value in edit mode
//                screen.update();
//                clickSound();
//            }
//
//            // Go to the edit mode
//            if(r_button.click()) {
//                isEdited = !isEdited; // Switch edit mode
//
//                // No edit mode in start screen
//                if(currentSettingMode == start) isEdited = false;
//
//                // When switch edit mode, update current data from settings on display
//                settingsModeMessages[1] = String("Кол-во ч/л: ") + settings.sugar_count_default;
//                settingsModeMessages[2] = String("Температура: ") + settings.tea_temp_default;
//
//                // Display overlap message
//                screen.setOverlap(isEdited); // Overlap message by new value in edit mode
//                screen.update();
//
//                editSound();
//                
//                // Update settings data in EEPROM when switch edit mode off
//                if(!isEdited) {
//                    EEPROM.put(0, settings);
//                    Serial.println(settings.sugar_spoon_time);
//                }
//            }
//
//            // Change default sugar count
//            if (currentSettingMode == sugar_count_default && isEdited) {
//                screen.setHeader("Ч/л сахара");   // Change header on screen
//                lcd.setCursor(5, 1);              // For blink
//                int newValue = potentRange(0, 4); // Get value from potentiometer
//
//                // Update only when value changed (screen.update can be used at least once every 50ms)
//                if(settings.sugar_count_default != newValue){
//                    settings.sugar_count_default = newValue;                     // Change local settings data
//                    screen.setOverlapMessage(String(newValue) + String(" шт.")); // Set new data in overlap message
//                    screen.update();
//                }
//            }
//
//            // Change default tea temperature
//            if (currentSettingMode == tea_temp_default && isEdited) {
//                screen.setHeader("Температура");     // Update header
//                lcd.setCursor(4, 1);                 // For blink
//                int newValue = potentRange(20, 100); // Get value from potentiometer
//
//                // Update only when value changed
//                if(settings.tea_temp_default != newValue){
//                    settings.tea_temp_default = newValue;                       // Change local settings data
//                    screen.setOverlapMessage(String(newValue) + String("°C"));  // Set new data in overlap message
//                    screen.update();
//                }
//            }
//
//            // Change sugar dispenser time
//            if (currentSettingMode == set_sugar_time && isEdited) {
//                screen.setHeader("Sugar time");         // Change header
//                screen.setOverlapMessage("Hold right"); // Add a hint by overlap
//                lcd.setCursor(10, 1);                   // For blink
//
//                // By hold turn on the sugar dispenser
//                // And count time of work of dispenser, save it to settings.
//                // It's time of work dispenser for getting one spoon of sugar
//                static unsigned long sugar_dispenser_start_time = millis();
//                if(r_button.hold()) {
//                    lcd.setCursor(2, 1); // Blink cursor position
//
//                    settings.sugar_spoon_time = millis() - sugar_dispenser_start_time;         // Work time in ms
//                    int sugar_dispenser_secs = (millis() - sugar_dispenser_start_time) / 1000; // Work time in secs
//                    screen.setOverlapMessage(String(sugar_dispenser_secs) + 's');              // Display work time in secs
//
//                    digitalWrite(sugar_dispenser_pin, HIGH);
//                } else {
//                    sugar_dispenser_start_time = millis(); // Before hold set start time
//                    digitalWrite(sugar_dispenser_pin, LOW);
//                }
//
//                // Update data every second
//                if(millis() - screenUpdateTimer > 1000) {
//                    screenUpdateTimer = millis();
//                    screen.update();
//                }
//            }
//
//            // Change water pump time
//            if (currentSettingMode == set_pump_time && isEdited) {
//                screen.setHeader("Pump time");          // Change header
//                screen.setOverlapMessage("Hold right"); // Add a hint by overlap
//                lcd.setCursor(10, 1);                   // For blink
//
//                // By hold turn the water pump on
//                // And count time of work of pump, save it to settings.
//                // It's time of work pump for getting one cup of water
//                static unsigned long pump_start_time = millis();
//                if(r_button.hold()) {
//                    lcd.setCursor(2, 1);  // Blink cursor position
//    
//                    settings.cup_pump_time = millis() - pump_start_time; // Work time in ms
//                    int pump_secs = (millis() - pump_start_time) / 1000; // Work time in secs
//                    screen.setOverlapMessage(String(pump_secs) + 's');   // Display work time in secs
//
//                    digitalWrite(hot_pump_pin, HIGH);
//                } else {
//                    pump_start_time = millis(); // Before hold set start time
//                    digitalWrite(hot_pump_pin, LOW);
//                }
//
//                // Update data every 500ms
//                if(millis() - screenUpdateTimer > 1000) {
//                    screenUpdateTimer = millis();
//                    screen.update();
//                }
//            }
//
//
//            // Go to the normal mode by hold two buttons
//            if(l_button.hold() && r_button.hold() && millis() - menu_delay_timer_2 > 1000){
//                isEdited = false;         // Switch off edit mode
//                screen.setOverlap(false); // Remove overlap
//                
//                menu_delay_timer_1 = millis(); // Update delay menu timer
//                currentMode = normalMode;      // Change mode
//                screen.update();
//                changeSound();
//                break;
//            }
//            
//            break; 
//
//        // --- Brewing Mode --- //
//        case brewingMode:
//            break;
//    }
//
//    // Add blink on display when edit mode is on
//    if(isEdited) screen.blink();
//    else screen.noBlink();
//}

/**
 * Every second get new value of the temperature
 */
void getTemperature(){
//    ds.requestTemp();
//    if(ds.readTemp()) {
//       Serial.println(ds.getTemp());
//      current_temp = ds.getTemp();
//    }
  
//    static unsigned long last_temp_request = 0;
//    if(millis() - last_temp_request > 1000) {
//        last_temp_request = millis();
//        if(ds.readTemp()) {
//            Serial.println(ds.getTemp());
////            current_temp = ds.getTemp();
//        } else Serial.println("error");
//      
//        ds.requestTemp();
//    }

ds.requestTemp();
  
  // вместо delay используй таймер на millis(), пример async_read
  delay(1000);
  
  // проверяем успешность чтения и выводим
  if (ds.readTemp()) Serial.println(ds.getTemp());
  else Serial.println("error");
}

//int potentRange(int min, int max) {
//    int value = analogRead(potent_pin);       // Get value from potent
//    return map(value, 0, 1024, min, max + 1); // Cut it from min to max
//}

/**
 * Control the stages of making tea:
 * - Kettle
 * - Water pump
 * - Sugar dispenser
 * - Mixer
 */
//void teaProcess(){
//    static uint8_t stage = 0;     // Current brewing stage
//    static long timer = millis(); // For delays in stages
//
//    // Stop making tea by start button click
//    if(r_button.click() && stage != 4){
//        stage = 4; // Go to the last stage
//        screen.update();
//        timer = millis();
//        changeSound();
//    }
//    
//    switch (stage) {
//      // Kettle
//      case 0: {
//        digitalWrite(kettle_relay_pin, HIGH);
//
//        screen.setHeader("Нагрев");
//        screen.setInfo(String("(") + current_temp + String("°C)"), 10);
//        screen.setMessage("Ожидайте...");
//
//        // Every second update current temp on display
//        static unsigned long updateTempTimer = millis();
//        if(millis() - updateTempTimer > 1000) {
//            screen.updateInfo();
//            updateTempTimer = millis();
//        }
//
//        // Go to the next stage when the desired temperature has been reached
//        if(current_temp >= tea_temp){
//            stage++;
//            screen.setInfo("", 15); // Clear ingo message (current water temp)
//            screen.update();
//            screen.updateInfo();
//            timer = millis();
//            digitalWrite(kettle_relay_pin, LOW);
//        }
//        
//        break;
//      }
//
//      // Pump
//      case 1:
//        screen.setHeader("Наполнение");
//
//        digitalWrite(hot_pump_pin, HIGH); // Turn the pump on
//
//        // Wait for end of timer of one cup time * cups count
//        if(millis() - timer > settings.cup_pump_time * cups_count) {
//            stage++;
//            screen.update();
//            timer = millis();
//            digitalWrite(hot_pump_pin, LOW);
//        }
//        
//        break;
//
//      // Turn on the mixer for prepare the spn for sugar dosing
//      case 2:
//          screen.setHeader("Наполнено");
//  
//          digitalWrite(mixer_pin, HIGH);
//  
//          // Start spining for 2.5 seconds
//          if(millis() - timer > 2500) {
//              stage++;
//              screen.update();
//              timer = millis();
//          }
//          
//          break;
//
//      // Sugar
//      case 3: {
//          static unsigned long fanTimer1 = millis();
//          static unsigned long fanTimer2 = millis();
//        
//          // --- No sugar --- //
//          // Skip stage if there is no sugar
//          if(sugar_count == 0) {
//              if(millis() - fanTimer1 > 500 && millis() - fanTimer1 < 550) screen.update();
//              if(millis() - fanTimer2 > 500 && millis() - fanTimer2 < 550) screen.update();
//              
//              if(millis() - fanTimer1 < 500){
//                  screen.setHeader("      (x)");
//                  fanTimer2 = millis();
//              } else {
//                  screen.setHeader("      (+)");
//
//                  if(millis() - fanTimer2 > 500){
//                      fanTimer1 = millis();
//                  } 
//              }
//            
//              // And wait the mixer time ends for tea withou mixing sugar 
//              if(millis() - timer > settings.mixer_time) {
//                  stage++;
//                  screen.setInfo("", 15);
//                  screen.update();
//                  screen.updateInfo();
//                  endMelody();
//                  timer = millis();
//                  digitalWrite(mixer_pin, LOW);
//              }
//              
//              break;
//          }
//
//          // --- Dispense sugar --- //
//          // Get total dispense time
//          unsigned long totalTime = settings.sugar_spoon_time * sugar_count * cups_count;
//
//          screen.setHeader("");
//
//          // Display current count of sugar
//          if(millis() - timer < totalTime)
//              screen.setInfo(
//                  String("Сахар: ") +
//                  String(int((float)(millis() - timer + 300) / totalTime * sugar_count * cups_count)) +
//                  String("ч/л")
//              , 0);
//
//          // Every second update current sugar count
//          static unsigned long updateSugarCountTimer = millis();
//          if(millis() - updateSugarCountTimer > 1000) {
//              screen.updateInfo();
//              updateSugarCountTimer = millis();
//          }
//  
//          // Turn on the sugar dispenser
//          digitalWrite(sugar_dispenser_pin, HIGH);
//  
//          // Go to the next stage after end of dosing sugar and end of mixing tea
//          if(millis() - timer > totalTime) {
//              static unsigned long mixer_timer = millis();
//
//              // Stop sugar dosing
//              digitalWrite(sugar_dispenser_pin, LOW);
//
//              if(millis() - mixer_timer > settings.mixer_time * cups_count) {
//                  timer = millis();
//                  endMelody();
//                  stage++;
//                  screen.setInfo("", 15);
//                  screen.update();
//                  screen.updateInfo();
//                  digitalWrite(mixer_pin, LOW); // Stop mixer
//              }
//          }
//      }
//        
//      break;
//
//      // Tea is done
//      case 4:
//          // End message on screen
//          screen.setHeader("Готово!");
//          screen.setMessage("OK");
//
//          // Disable all for sure
//          digitalWrite(kettle_relay_pin, LOW);
//          digitalWrite(mixer_pin, LOW);
//          digitalWrite(hot_pump_pin, LOW);
//          digitalWrite(sugar_dispenser_pin, LOW);
//
//          // Return to normal mode by click left button
//          if(l_button.click()) {
//              screen.update();
//              currentMode = normalMode;
//              stage = 0;
//              clickSound();
//          }
//          break;
//
//      default:
//          break;
//    }
//}

//void boil(){
//    screen.setHeader("Кипячение");
//    screen.setInfo(String("(") + current_temp + String("°C)"), 10);
//    screen.setMessage("СТОП");
//
//    // Every second update info on LCD screen to display current temp
//    static unsigned long updateTempTimer = millis();
//    if(millis() - updateTempTimer > 1000) {
//        screen.updateInfo();
//        updateTempTimer = millis();
//    }
//
//    if(l_button.click()) {
//        screen.update();
//        currentMode = normalMode;
//        digitalWrite(kettle_relay_pin, LOW);
//        changeSound();
//        return;
//    }
//  
//    // Switch off the kettle when the temperature is riched
//    if(current_temp >= 95) {
//        digitalWrite(kettle_relay_pin, LOW); // Switch off the kettle
//        currentMode = normalMode;            // Change mode
//        screen.update();                     // Update header on display
//        return;
//    }
//    
//    digitalWrite(kettle_relay_pin, HIGH);
//}

int clickMelody[] = {40};
int clickDurations[] = {8};
void clickSound() {
//    buzzer.playMelody(clickMelody, clickDurations, 1);
}

int changeMelody[] = {150, 100};
int changeDurations[] = {4, 8};
void changeSound() {
//    buzzer.playMelody(changeMelody, changeDurations, 2);
}

int editMelody[] = {50, 60};
int editDurations[] = {4, 8};
void editSound() {
//    buzzer.playMelody(editMelody, editDurations, 2);
}

//int melody[] = {
//    NOTE_DS5,NOTE_D5,NOTE_B4,NOTE_A4,NOTE_B4,
//    NOTE_E4,NOTE_G4,NOTE_DS5, NOTE_D5,NOTE_G4,NOTE_B4,
//    NOTE_B4,NOTE_FS5,NOTE_F5,NOTE_B4,
//};
//int durations[] = {
//    8,2,8,8,1,
//    8,4,8,4,8,8,
//    8,8,4,
//};
//
//void endMelody() {
//    int length = sizeof(durations) / sizeof(int);
//    buzzer.playMelody(melody, durations, length); // playing
//}
