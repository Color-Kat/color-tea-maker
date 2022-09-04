#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h> // For lcd display

LCD_1602_RUS _lcd(0x27, 16, 2);

class Screen
{
public:
    Screen()
    {
        _lcd.init();
        _lcd.backlight();
        _lcd.setCursor(0, 0);
        _lcd.print("Чай машина OC");
        _lcd.setCursor(4, 1);
        _lcd.print("Loading...");
    }

    void init() {
        _lcd.init();
        _lcd.backlight();
        _lcd.setCursor(0, 0);
        _lcd.print("Работает");
        _lcd.setCursor(4, 1);
        _lcd.print("Loading...");
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

//        _lcd->setCursor(1, 0);
//        _lcd->print(_header); // Display header message in first row
//
//        _lcd->setCursor(1, 0);
//        _lcd->print(_message); // Display message in second row
    }

private:
//    LCD_1602_RUS *_lcd;

//    bool _prev_state;
//    bool _state;
//    unsigned long _header_timer;
//    unsigned long _info_timer;
//    char *_message;
//    char *_header;
//    byte _info[4];
};
