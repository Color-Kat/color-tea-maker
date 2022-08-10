/* --- PINS --- */
#define kettle_relay_pin A5  // Relay of kettle
#define hot_pump_pin A4      // Hot water
#define motor_pin_1 2        // Sugar dispenser
#define motor_pin_2 3        // Sugar dispenser
#define mixer_relay_pin A1   // Mixer pin
#define switch_button_pin 12 // Switch button pin

/* --- Timers --- */
int cup_pump_time = 5 * 1000;
int sugar_spoon_time = 4 * 1000;
int mixer_time = 5 * 1000;

class button {
  public:
    button(byte pin) {
      _pin = pin;
      pinMode(_pin, INPUT_PULLUP);
    }
    bool click() {
      bool btnState = digitalRead(_pin);
      if (!btnState && !_flag && millis() - _tmr >= 300) {
        _flag = true;
        _tmr = millis();
        return true;
      }
      if (!btnState && _flag && millis() - _tmr >= 300) {
        _tmr = millis();
        return true;
      }
      if (btnState && _flag) {
        _flag = false;
        _tmr = millis();
      }
      return false;
    }

  private:
    byte _pin;
    uint32_t _tmr;
    bool _flag;
};

button sw_button(switch_button_pin); // Class button, object sw_button

void setup()
{
  Serial.begin(9600);

  pinMode(kettle_relay_pin, OUTPUT);
  digitalWrite(kettle_relay_pin, LOW);
  
  pinMode(hot_pump_pin, OUTPUT);
  digitalWrite(hot_pump_pin, LOW);
  
  pinMode(mixer_relay_pin, OUTPUT);
  digitalWrite(mixer_relay_pin, LOW);
  
  pinMode(motor_pin_1, OUTPUT);
  pinMode(motor_pin_2, OUTPUT);
  digitalWrite(motor_pin_1, LOW);
  digitalWrite(motor_pin_2, LOW);
}

void loop()
{
  static uint8_t stage = 0;
  static long timer = millis();
  
  // Switch state of making tea
  if (sw_button.click()) {
    stage++;
    if (stage > 4) stage = 0;

    Serial.println(stage);
  }

   switch (stage) {
      // Kettle
      case 0:
        Serial.println("Чайник включён");
        digitalWrite(kettle_relay_pin, HIGH);
        timer = millis();
        break;

      // Pump
      case 1:
        Serial.println("Помпа включена");
        digitalWrite(kettle_relay_pin, LOW);

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
        break;

      default:
        break;
    }
}
