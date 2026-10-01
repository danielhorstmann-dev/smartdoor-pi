#ifndef TUERSTEUERUNG_H
#define TUERSTEUERUNG_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "Tuerkontakt.h"

class Tuersteuerung {

  private:
    int servoPin;
    bool tuerEntriegelt;
    bool tuerWurdeGeoeffnet;

    unsigned long entriegeltSeit;
    unsigned long oeffnungsTimeout;

    Tuerkontakt& tuerKontakt;

    Servo servo;
  
  public:
    Tuersteuerung(int servoPin, Tuerkontakt& tuerKontakt);

    void begin();
    void pruefen();

    void entriegeln();
    void verriegeln();

    bool istEntriegelt();

};

#endif