#ifndef TUERSTEUERUNG_H
#define TUERSTEUERUNG_H

#include <Arduino.h>
#include <ESP32Servo.h>

class Tuersteuerung {

  private:
    int servoPin;
    bool tuerEntriegelt;

    unsigned long entriegeltSeit;
    unsigned long offenDauer;
    bool automatischVerriegeln;

    Servo servo;
  
  public:
    Tuersteuerung(int servoPin);

    void begin();
    void pruefen();

    void entriegeln();
    void verriegeln();

    bool istEntriegelt();

};

#endif