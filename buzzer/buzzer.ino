#include <ezBuzzer.h>


const int BUZZER_PIN = 9;

ezBuzzer buzzer(BUZZER_PIN); // create ezBuzzer object that attach to a pin;

// notes in the melody:
//int melody[] = {
//  NOTE_E5, NOTE_E5, NOTE_E5,
//  NOTE_E5, NOTE_E5, NOTE_E5,
//  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
//  NOTE_E5,
//  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
//  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
//  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
//  NOTE_D5, NOTE_G5
//};
//
//// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
//int durations[] = {
//  8, 8, 4,
//  8, 8, 4,
//  8, 8, 8, 8,
//  2,
//  8, 8, 8, 8,
//  8, 8, 8, 16, 16,
//  8, 8, 8, 8,
//  4, 4
//};

//int melody[] = {
//  NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, 
//  NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5, 
//  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5, 
//  NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
//};
//
//// The note duration, 8 = 8th note, 4 = quarter note, etc.
//int durations[] = {
//  8, 8, 8, 4, 4, 4, 
//  4, 5, 8, 8, 8, 8, 
//  8, 8, 8, 4, 4, 4, 
//  4, 5, 8, 8, 8, 8
//};

//int melody[] = {
//  // Nokia Ringtone 
//  
//  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
//  NOTE_CS5,  NOTE_B4,  NOTE_D4,  NOTE_E4, 
//  NOTE_B4,  NOTE_A4,  NOTE_CS4, NOTE_E4,
//  NOTE_A4, 
//};
//int durations[] = {
//  8,8,4,4,8,8,4,4,8,8,4,4,2
//};

int melody[] = {
NOTE_DS5,NOTE_D5,NOTE_B4,NOTE_A4,NOTE_B4,
NOTE_E4,NOTE_G4,NOTE_DS5, NOTE_D5,NOTE_G4,NOTE_B4,
NOTE_B4,NOTE_FS5,NOTE_F5,NOTE_B4,
};


// note duration: 1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, etc.
int durations[] = {
8,2,8,8,1,
8,4,8,4,8,8,
8,8,4,
};

void setup () {
  Serial.begin(9600);
  int length = sizeof(durations) / sizeof(int);
  buzzer.playMelody(melody, durations, length); // playing
}

void loop() {
    buzzer.loop(); // MUST call the buzzer.loop() function in loop()
     
}


void sound () {
 
}
