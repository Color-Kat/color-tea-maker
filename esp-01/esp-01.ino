/*
   -- New project --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.8 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.11.1 or later version;
     - for iOS 1.9.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// определение режима соединения и подключение библиотеки RemoteXY 
#define REMOTEXY_MODE__ESP8266_SOFTSERIAL_POINT
#include <SoftwareSerial.h>

#include <RemoteXY.h>

// настройки соединения 
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600
#define REMOTEXY_WIFI_SSID "TeaMaker"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// конфигурация интерфейса  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 159 bytes
  { 255,1,0,5,0,152,0,16,186,1,71,56,16,18,32,32,3,51,16,70,
  0,0,160,65,0,0,200,66,0,0,160,65,0,0,32,65,0,0,0,64,
  16,208,162,208,181,208,186,209,131,209,137,208,176,209,143,32,209,130,208,181,
  208,188,208,191,208,181,209,128,208,176,209,130,209,131,209,128,208,176,0,137,
  0,0,0,0,0,0,72,66,95,0,0,72,66,0,0,150,66,38,0,0,
  150,66,0,0,200,66,129,0,11,7,36,6,16,208,167,208,176,208,185,32,
  208,188,208,176,209,136,208,184,208,189,208,176,0,70,20,50,8,4,4,38,
  134,0,2,0,22,68,21,10,2,26,16,31,79,78,0,79,70,70,0 };
  
// структура определяет все переменные и события вашего интерфейса управления 
struct {

    // input variables
  uint8_t isBrewing; // =1 если переключатель включен и =0 если отключен 

    // output variables
  float current_remp;  // oт 20 до 100 
  uint8_t is_brewing; // led state 0 .. 1 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup() 
{
  RemoteXY_Init (); 
  
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  
  // TODO you loop code
  // используйте структуру RemoteXY для передачи данных
  // не используйте функцию delay() 


}
