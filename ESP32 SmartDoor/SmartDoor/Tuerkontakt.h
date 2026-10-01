#ifndef TUERKONTAKT_H
#define TUERKONTAKT_H

#include <Arduino.h>

class Tuerkontakt {

  private:
    int kontaktPin;
    bool tuerOffen;

  public:
    Tuerkontakt(int kontaktPin);

    void begin();
    void pruefen();

    bool istOffen();
    bool istGeschlossen();
};

#endif