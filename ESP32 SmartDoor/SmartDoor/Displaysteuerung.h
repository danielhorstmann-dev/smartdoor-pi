#ifndef DISPLAYSTEUERUNG_H
#define DISPLAYSTEUERUNG_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Displaysteuerung {

  private:
    LiquidCrystal_I2C lcd;

    void leeren();

  public:
    Displaysteuerung();

    void begin();

    void bereit();
    void pinEingabe(int anzahl);
    void zugangErlaubt();
    void zugangVerweigert();
    void tuerVerriegelt();
    void tuerOffen();
    void tuerGeoeffnet();
    void tuerGeschlossen();
    void gesperrt(unsigned long sekunden);
};

#endif