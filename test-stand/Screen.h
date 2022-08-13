#include <GyverTM1637.h> // For led display

class Screen
{
public:
    Screen(GyverTM1637 *disp)
    {
        _header_timer = millis();
        _info_timer = millis();

        _prev_state = false;
        _state = false;

        _disp = disp;
        _disp->clear();
        _disp->brightness(7); // Brithness, 0 - 7 (min - max)
        _disp->displayByte(_t, _e, _a, _empty);
    }

    void render()
    {
        // Display header only if header is updated
        if (_state != _prev_state && millis() - _header_timer < 1500){
            _disp->displayByte(_header); // Display header of menu
            return;
        }
        else if (_state != _prev_state)
            _state = _prev_state;

        if (millis() - _info_timer < 2500){
            _disp->displayByte(_info); // Display info data
            return;
        }

        _disp->displayByte(_message); // Display some message in menu main screen
    }

    // Update current state. Call when header is changed
    void updateState(){
        _state = !_prev_state;
    }

    // Set new message for menu
    void setMessage(byte byte_1, byte byte_2, byte byte_3, byte byte_4) {
        _message[0] = byte_1;
        _message[1] = byte_2;
        _message[2] = byte_3;
        _message[3] = byte_4;
    }

    // Set new message for numbers
    void setMessage(int number, byte sign) {
        byte d_1 = number / 100 > 0 ? toHex(number / 100) : _empty; // Display only if it's not 0
        byte d_2 = (number % 100 / 10 > 0 || number / 100 > 0) ? toHex(number % 100 / 10) : _empty; // Don't display if first number is 0
        byte d_3 = toHex(number % 10);

        setMessage(d_1, d_2, d_3, sign);
    }

    // Set new header for meny
    void setHeader(byte byte_1, byte byte_2, byte byte_3, byte byte_4) {
        // Update header timer until header is displayed
        if (_state == _prev_state)
            _header_timer = millis();

        _header[0] = byte_1;
        _header[1] = byte_2;
        _header[2] = byte_3;
        _header[3] = byte_4;
    }

    void setInfo(byte byte_1, byte byte_2, byte byte_3, byte byte_4) {
        // Update info timer
        if (_state == _prev_state)
            _info_timer = millis();

        _info[0] = byte_1;
        _info[1] = byte_2;
        _info[2] = byte_3;
        _info[3] = byte_4;
    }

    void setNumber(int number, byte sign) {
        byte d_1 = number / 100 > 0 ? toHex(number / 100) : _empty; // Display only if it's not 0
        byte d_2 = (number % 100 / 10 > 0 || number / 100 > 0) ? toHex(number % 100 / 10) : _empty; // Don't display if first number is 0
        byte d_3 = toHex(number % 10);

        setInfo(d_1, d_2, d_3, sign);
    }

    byte toHex(int number)
    {
        if (number == 0)
            return _0;
        if (number == 1)
            return _1;
        if (number == 2)
            return _2;
        if (number == 3)
            return _3;
        if (number == 4)
            return _4;
        if (number == 5)
            return _5;
        if (number == 6)
            return _6;
        if (number == 7)
            return _7;
        if (number == 8)
            return _8;
        if (number == 9)
            return _9;
    }

private:
    GyverTM1637 *_disp;

    bool _prev_state;
    bool _state;
    unsigned long _header_timer;
    unsigned long _info_timer;
    byte _message[4];
    byte _header[4];
    byte _info[4];
};
