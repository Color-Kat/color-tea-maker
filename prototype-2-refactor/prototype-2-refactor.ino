

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

void setup()
{
    Serial.begin(9600);
    Serial.println(123);

    pinMode(sugar_button_pin, INPUT);

    pinMode(mixer_pin, OUTPUT);
}

void loop() {
    boolean isActive = digitalRead(sugar_button_pin);

    if(isActive) {
        digitalWrite(mixer_pin, HIGH);
    } else {
      digitalWrite(mixer_pin, LOW);
    }
}
