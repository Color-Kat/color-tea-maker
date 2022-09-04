#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>

class Screen
{
public:
    Screen(LCD_1602_RUS *lcd)
    {
        _lcd = lcd;
        _update = true;
    }

    void init() {
        _lcd->init();
        _lcd->backlight();
        _lcd->setCursor(0, 0);
        _lcd->print("Чай машина OC");
        _lcd->setCursor(4, 1);
        _lcd->print("Loading...");
    }

    void update() {
        _update = true;
    }
 
    void render()
    {
        if(_update) {
          _lcd->clear();
          _lcd->setCursor(0, 0);
          _lcd->print(_header);
  
          _lcd->setCursor(0, 1);
          _lcd->print(_message);

          _update = false;
        }
    }

    void setHeader(String header) {
        _header = header;
    }

    void setMessage(String message) {
        _message = message;
    }

private:
    LCD_1602_RUS *_lcd;
    boolean _update;
//    bool _prev_state;
//    bool _state;
//    unsigned long _header_timer;
//    unsigned long _info_timer;
      String _message;
      String _header;
//    byte _info[4];
};
