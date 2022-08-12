#include <microDS18B20.h> // For thermometer DS18b20
#include <GyverTM1637.h> // For led display
#include <EncButton.h>

/* --- PINS --- */
#define kettle_relay_pin 4   // Relay of kettle
#define termometr_pin A5     // Relay of kettle
#define hot_pump_pin 5       // Hot water
#define motor_pin_1 2        // Sugar dispenser
#define motor_pin_2 3        // Sugar dispenser
#define mixer_relay_pin A1   // Mixer pin
#define display_CLK A3       // CLK pin of led display
#define display_DIO A4       // CLK pin of led display

#define temp_button_pin 12   // Button for change the temperature
#define sugar_button_pin 11  // Button for change number of sugar spoons

/* --- Timers --- */
int cup_pump_time = 11 * 1000;
int sugar_spoon_time = 4 * 1000;
int mixer_time = 5 * 1000;

// --- Libs --- //

// Init buttons
EncButton<EB_TICK, temp_button_pin> temp_button;   
EncButton<EB_TICK, sugar_button_pin> sugar_button; 
//button temp_button(temp_button_pin);
//button sugar_button(sugar_button_pin);

// Connect thermometer DS18b20
MicroDS18B20<termometr_pin> ds; 

// Connect led display
GyverTM1637 disp(display_CLK, display_DIO);

// --- Code --- //

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
  
    // Led display
    disp.clear();
    disp.brightness(7);  // Brithness, 0 - 7 (min - max)
    disp.displayByte(_t, _e, _a, _empty);
}

int current_temp = 30; // Current temperature
int tea_temperature = 30; // Desired temperature
int sugar_count = 0; // Number of spoons of sugar

// --- Modes of tea machine --- //
enum modes {
    normal, // Tea is not brewing, we can change params
    brewing, // Tea is brewing
    settings, // Edit settings mode (change timers for components)
};
modes currentMode = normal;

void loop()
{
     buttons();
  
     /* --- Temperature --- */
     getTemperature();
    
     /* --- Stages of making tea --- */
     if(currentMode == brewing)
        teaProcess();
}

void buttons() {
    temp_button.tick();
    sugar_button.tick();
  
    switch (currentMode) {
        // --- Normal Mode --- //
        case normal: 
            // Go to the setting mode

            if(!blinkDisplay(_S, _t, _O, _P)){
                disp.displayByte(_H, _i, _empty, _empty);
                if(temp_button.hold() && sugar_button.hold()){
                    currentMode = settings;
                    break;
                }
            }
        
            // Change temp by button
            if (temp_button.click()) {
              tea_temperature += 5;
              if(tea_temperature > 100)
                  tea_temperature = 30;

              disp.clear();
              disp.displayInt(tea_temperature);

              Serial.println(tea_temperature);
              break;
            }
          
            // Change number of sugar spoons by button
            if (sugar_button.click()) {
              sugar_count += 1;
              if(sugar_count > 4) sugar_count = 0;
              Serial.println(sugar_count);

              disp.clear();
              disp.displayInt(sugar_count);
              
              break;
            }
            
//            disp.clear();
//            disp.displayByte(_t, _e, _a, _empty);
                        
            break;

        // --- Brewing Mode --- //
        case brewing:
            break;
            
        // --- Settings Mode --- //
        case settings:
            Serial.println("Settings mode is on");

            if(!blinkDisplay(_S, _e, _t, _empty)){
                disp.displayByte(_H, _i, _empty, _empty);

                // Go to the normal mode
                if(temp_button.hold() && sugar_button.hold()){
                    currentMode = normal;
                    break;
                }
            }
            
            break; 
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
    
    switch (stage) {
      // Kettle
      case 0:
        Serial.println("Чайник включён");
        digitalWrite(kettle_relay_pin, LOW);

        if(current_temp >= tea_temperature){
          stage++;
          timer = millis();
          digitalWrite(kettle_relay_pin, HIGH);
        }
        break;

      // Pump
      case 1:
        Serial.println("Помпа включена");

        digitalWrite(hot_pump_pin, HIGH);
        
        Serial.println(millis(), timer);
        Serial.println(cup_pump_time);
        if(millis() - timer > cup_pump_time) {
            stage++;
            timer = millis();
            digitalWrite(hot_pump_pin, LOW);
            Serial.println("Условие");
        }
        
        break;

      // Sugar
      case 2:
        Serial.println("Дозатор сахара включён");

        digitalWrite(motor_pin_1, LOW);
        digitalWrite(motor_pin_2, HIGH);

        if(millis() - timer > sugar_spoon_time) {
          stage++;
          timer = millis();
          digitalWrite(motor_pin_1, LOW);
          digitalWrite(motor_pin_2, LOW);
        }
        
        break;

      // Mixer
      case 3:
        Serial.println("Включаем мешалку");

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
        currentMode = normal;
        break;

      default:
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

bool blinkDisplay(byte byte_1, byte byte_2, byte byte_3, byte byte_4){
  static unsigned long timer = millis();
  
  if(millis() - timer < 1000) {
    disp.displayByte(byte_1, byte_2, byte_3, byte_4);
    return true;
  }
  
  return false;
}
