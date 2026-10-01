#ifndef STATUSANZEIGE_H
#define STATUSANZEIGE_H

#include <Arduino.h>

class Statusanzeige {

  private:
    int rotPin;
    int gruenPin;
    int blauPin;

    unsigned long statusSeit;
    unsigned long statusDauer;
    bool statusZeitAktiv;

    void farbeSetzen(bool rot, bool gruen, bool blau);

  //  int buzzerPin;

  //  unsigned long buzzerBis;
  //  bool buzzerAktiv;

  //  void buzzerStarten(unsigned long dauer);

  public:
    Statusanzeige(int rotPin, int gruenPin, int blauPin);

    void begin();
    void pruefen();

    void erfolg();
    void fehler();
    void bereit();
    void gesperrt();

    void aus();
    void rot();
    void gruen();
    void blau();
};

#endif