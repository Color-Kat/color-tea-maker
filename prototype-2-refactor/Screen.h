#define _LCD_TYPE 1
//#include <LCD_1602_RUS_ALL.h> // For lcd display

class Screen
{
public:
    Screen(LCD_1602_RUS *lcd)
    {
//        _header_timer = millis();
//        _info_timer = millis();
//
//        _prev_state = false;
//        _state = false;

//        Serial.begin(9600);
//        Serial.println("dAS");

        _lcd = lcd;
        _lcd->init();
        _lcd->backlight();
        _lcd->setCursor(0, 0);
        _lcd->print("Чай машина OC");
        _lcd->setCursor(4, 1);
        _lcd->print("Loading...");
    }
 
    void render()
    {
//        // Display header only if header is updated
//        if (_state != _prev_state && millis() - _header_timer < 1500){
//            _disp->displayByte(_header); // Display header of menu
//            return;
//        }
//        else if (_state != _prev_state)
//            _state = _prev_state;
//
//        if (millis() - _info_timer < 2500){
//            _disp->displayByte(_info); // Display info data
//            return;
//        }

        _lcd->setCursor(1, 0);
        _lcd->print(_header); // Display header message in first row

        _lcd->setCursor(1, 0);
        _lcd->print(_message); // Display message in second row
    }

    // Update current state. Call when header is changed
    void updateState(){
//        _state = !_prev_state;
    }

    // Set new message for menu
    void setMessage(const char *_str) {
        _message = _str;
    }

    // Set new message for numbers
//    void setMessage(int number, byte sign) {
//        byte d_1 = number / 100 > 0 ? toHex(number / 100) : _empty; // Display only if it's not 0
//        byte d_2 = (number % 100 / 10 > 0 || number / 100 > 0) ? toHex(number % 100 / 10) : _empty; // Don't display if first number is 0
//        byte d_3 = toHex(number % 10);
//
//        setMessage(d_1, d_2, d_3, sign);
//    }

    // Set new header for meny
    void setHeader(const char *_str) {
        // Update header timer until header is displayed
//        if (_state == _prev_state)
//            _header_timer = millis();

        _header = _str;
    }

//    void setInfo(byte byte_1, byte byte_2, byte byte_3, byte byte_4) {
//        // Update info timer
//        if (_state == _prev_state)
//            _info_timer = millis();
//
//        _info[0] = byte_1;
//        _info[1] = byte_2;
//        _info[2] = byte_3;
//        _info[3] = byte_4;
//    }
//
//    void setNumber(int number, byte sign) {
//        byte d_1 = number / 100 > 0 ? toHex(number / 100) : _empty; // Display only if it's not 0
//        byte d_2 = (number % 100 / 10 > 0 || number / 100 > 0) ? toHex(number % 100 / 10) : _empty; // Don't display if first number is 0
//        byte d_3 = toHex(number % 10);
//
//        setInfo(d_1, d_2, d_3, sign);
//    }

private:
    LCD_1602_RUS *_lcd;

//    bool _prev_state;
//    bool _state;
//    unsigned long _header_timer;
//    unsigned long _info_timer;
    char *_message;
    char *_header;
//    byte _info[4];
};
