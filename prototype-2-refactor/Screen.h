#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>

class Screen
{
public:
    Screen(LCD_1602_RUS *lcd)
    {
        _lcd = lcd;
        _update = true;
        _updateInfo = true;
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
         _updateTimer = millis();
    }

    void updateInfo() {
        _updateInfo = true;
    }
 
    void render()
    {
        if(millis() - _updateTimer > 50 && _update) {
          _lcd->clear();
          _lcd->setCursor(0, 0);
          _lcd->print(_header);
  
          _lcd->setCursor(0, 1);
          _lcd->print(_message);

          _update = false;
        } 
        
        if(_updateInfo) {
            Serial.println(_info);
            _lcd->setCursor(_infoCursor, 0);
            _lcd->print(_info);
            _updateInfo = false;
        }
    }

    void setHeader(String header) {
        _header = header;
    }

     void setInfo(String info, byte cursor) {
        _info = info;
        _infoCursor = cursor;
    }

    void setMessage(String message) {
        _message = message;
    }

private:
    LCD_1602_RUS *_lcd;
    boolean _update;
    boolean _updateInfo;

    unsigned long _updateTimer;
    
    String _message;
    String _header;
    String _info;
    unsigned int _infoCursor;
};
