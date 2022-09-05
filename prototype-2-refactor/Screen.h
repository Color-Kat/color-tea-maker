#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>

class Screen
{
public:
    Screen(LCD_1602_RUS *lcd)
    {
        Serial.begin(9600);
        _lcd = lcd;
        _update = true;
        _updateInfo = true;
        _isOverlap = false;
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
          Serial.println(_isOverlap);
          if(!_isOverlap) _lcd->print(_message); // Display the main message
          else _lcd->print(_overlapMessage);     // Display the overlap message

          _update = false;
        } 
        
        if(_updateInfo) {
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

    void setOverlapMessage(String message) {
        _overlapMessage = message;
    }

    void blink(){
        _lcd->blink();
    }

    void noBlink(){
        _lcd->noBlink();
    }

    // Show/hide the overlap message over the main message
    void setOverlap(boolean overlap){
        _isOverlap = overlap;
    }

private:
    LCD_1602_RUS *_lcd;
    boolean _update;
    boolean _updateInfo;
    unsigned long _updateTimer;

    boolean _isOverlap;
    
    String _message;
    String _overlapMessage;
    String _header;
    String _info;
    unsigned int _infoCursor;
};
