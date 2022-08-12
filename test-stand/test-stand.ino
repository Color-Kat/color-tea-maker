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

class Screen {
    public:
        Screen() {
            _header_timer = millis();
            _info_timer = millis();

            disp.clear();
            disp.brightness(7);  // Brithness, 0 - 7 (min - max)
        }

        void render() {
            if(millis() - _header_timer < 1000) {
                disp.displayByte(_header);
                return;
            } 

            if(millis() - _info_timer < 2000) {
                disp.displayByte(_info);
                return;
            } 
            
            disp.displayByte(_message);
        }

        void setMessage(byte byte_1, byte byte_2, byte byte_3, byte byte_4){
            _message[0] = byte_1;
            _message[1] = byte_2;
            _message[2] = byte_3;
            _message[3] = byte_4;
        }

         void setHeader(byte byte_1, byte byte_2, byte byte_3, byte byte_4){
            _header_timer = millis();
            _header[0] = byte_1;
            _header[1] = byte_2;
            _header[2] = byte_3;
            _header[3] = byte_4;
        }

        void setInfo(byte byte_1, byte byte_2, byte byte_3, byte byte_4){
            _info_timer = millis();
            _info[0] = byte_1;
            _info[1] = byte_2;
            _info[2] = byte_3;
            _info[3] = byte_4;
        }

    private:
        boolean _header_timer;
        boolean _info_timer;
        byte _header[];
        byte _message[];
        byte _info[];
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
  
    screen.setMessage(_t, _e, _a, _empty);
}

int current_temp = 30; // Current temperature
int tea_temperature = 30; // Desired temperature
int sugar_count = 0; // Number of spoons of sugar

// --- Display variables --- //
byte disp_message[] = {_t, _E, _a, _empty}; 
boolean disp_busy = true;
boolean header = true;
unsigned long blink_timer = millis(); // Timer for display

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
     
     screen.render();
}

unsigned long header_timer = millis();
unsigned long info_timer = millis();
void buttons() {
    temp_button.tick();
    sugar_button.tick();
  
    switch (currentMode) {
        // --- Normal Mode --- //
        case normal: 
            // First show the mode header
            screen.setHeader(_S, _t, _O, _P);
            screen.setMessage(_H, _i, _empty, _empty);

            // Go to the setting mode
            if(temp_button.hold() && sugar_button.hold()){
                currentMode = settings;
                break;
            }
        
            // Change temp by button
            if (temp_button.click()) {
              tea_temperature += 5;
              if(tea_temperature > 100)
                  tea_temperature = 30;

              screen.setInfo(_d, _i, _S, _P);

              Serial.println(tea_temperature);
              break;
            }
          
            // Change number of sugar spoons by button
            if (sugar_button.click()) {
              sugar_count += 1;
              if(sugar_count > 4) sugar_count = 0;
              
              Serial.println(sugar_count);
              screen.setInfo(_d, _i, _S, _P);
              
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
            if(temp_button.hold() && sugar_button.hold()){
                header_timer= millis();
                currentMode = normal;
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
