#include <microDS18B20.h> // For thermometer DS18b20
#include <GyverTM1637.h> // For led display
#include <EncButton.h> // For buttons
#include <EEPROM.h> // For EEPROM memory to save settings

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

// Connect thermometer DS18b20
MicroDS18B20<termometr_pin> ds; 

// Connect led display
GyverTM1637 disp(display_CLK, display_DIO);

// --- Code --- //
class Screen {
    public:
        Screen() {
            _header_timer = millis();
            _info_timer = millis();

            _prev_state = false;
            _state = false;

            disp.clear();
            disp.brightness(7);  // Brithness, 0 - 7 (min - max)
        }

        void render() {
            // Display header only if header is updated
            if(_state != _prev_state && millis() - _header_timer < 1500) {
                disp.displayByte(_header); // Display header of menu
                return;
            } else if(_state != _prev_state) _state = _prev_state;
            
            if(millis() - _info_timer < 2500) {
                disp.displayByte(_info); // Display info data
                return;
            } 

            disp.displayByte(_message); // Display some message in menu main screen
        }

        // Update current state. Call when header is changed
        void updateState(){
            _state = !_prev_state;
        }

        // Set new message for meny
        void setMessage(byte byte_1, byte byte_2, byte byte_3, byte byte_4){
            _message[0] = byte_1;
            _message[1] = byte_2;
            _message[2] = byte_3;
            _message[3] = byte_4;
        }

         // Set new header for meny
         void setHeader(byte byte_1, byte byte_2, byte byte_3, byte byte_4){
            // Update header timer until header is displayed
            if(_state == _prev_state) _header_timer = millis();
            
            _header[0] = byte_1;
            _header[1] = byte_2;
            _header[2] = byte_3;
            _header[3] = byte_4;
        }

        void setInfo(byte byte_1, byte byte_2, byte byte_3, byte byte_4){
            // Update info timer
            if(_state == _prev_state) _info_timer = millis();
            
            _info[0] = byte_1;
            _info[1] = byte_2;
            _info[2] = byte_3;
            _info[3] = byte_4;
        }

        byte toHex(int number){
            if(number == 0) return _0;
            if(number == 1) return _1;
            if(number == 2) return _2;
            if(number == 3) return _3;
            if(number == 4) return _4;
            if(number == 5) return _5;
            if(number == 6) return _6;
            if(number == 7) return _7;
            if(number == 8) return _8;
            if(number == 9) return _9;
        }

    private:
        bool _prev_state;
        bool _state;
        unsigned long _header_timer;
        unsigned long _info_timer;
        byte _message[4];
        byte _header[4];
        byte _info[4];
};

Screen screen;

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
  
    disp.displayByte(_t, _e, _a, _empty);
    screen.updateState(); // First update for display menu header
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
     buttons(); // Buttons actions
  
     getTemperature(); // Temperature=
    
     // Stages of making tea 
     if(currentMode == brewing)
        teaProcess();
     
     screen.render(); // Display some information
}

void buttons() {
    // Delay timers between switching menu modes
    static unsigned long menu_delay_timer_1 = millis();
    static unsigned long menu_delay_timer_2 = millis();
  
    temp_button.tick();
    sugar_button.tick();
  
    switch (currentMode) {
        // --- Normal Mode --- //
        case normal: 
            screen.setHeader(_S, _t, _O, _P); // Menu header
            screen.setMessage(_H, _i, _empty, _empty); // Menu main text

            // Go to the setting mode
            if(millis() - menu_delay_timer_1 > 1000 && temp_button.hold() && sugar_button.hold()){
                menu_delay_timer_2 = millis(); // Set delay between menu modes
                currentMode = settings;        // Change mode
                screen.updateState();          // Update header on display
                
                break;
            }
        
            // Change temp by button
            if (temp_button.click()) {
                tea_temperature += 5;
                
                if(tea_temperature > 100) // Max temp is 100
                    tea_temperature = 30;

                // Show temp on display
                byte d_1 = tea_temperature / 100 > 0 ? screen.toHex(tea_temperature / 100) : _empty;
                byte d_2 = screen.toHex(tea_temperature % 100 / 10);
                byte d_3 = screen.toHex(tea_temperature % 10);

                screen.setInfo(d_1, d_2, d_3, _t);
  
                break;
            }
          
            // Change number of sugar spoons by button
            if (sugar_button.click()) {
              sugar_count += 1;
              if(sugar_count > 4) sugar_count = 0;

              // Show count on display
              byte sugar_count_dig = screen.toHex(sugar_count);
              screen.setInfo(_empty, _empty, sugar_count_dig, _c);
              
              break;
            }
                        
            break;

        // --- Brewing Mode --- //
        case brewing:
            break;
            
        // --- Settings Mode --- //
        case settings:
            Serial.println("Settings mode is on");
           
            screen.setHeader(_S, _e, _t, _empty);
            screen.setMessage(_H, _i, _S, _empty);

            // Go to the normal mode
            if(millis() - menu_delay_timer_2 > 1000 && temp_button.hold() && sugar_button.hold()){
                menu_delay_timer_1 = millis();  // Set delay between menu modes
                currentMode = normal;           // Change mode
                screen.updateState();           // Update header on display
                break;
            }
            
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
