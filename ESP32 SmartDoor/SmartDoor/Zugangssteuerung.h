#ifndef ZUGANGSSTEUERUNG_H
#define ZUGANGSSTEUERUNG_H

#include <Arduino.h>
#include <Keypad.h>
#include <Preferences.h>

#include "Tuersteuerung.h"
#include "Tuerkontakt.h"
#include "Statusanzeige.h"
#include "Displaysteuerung.h"

class Zugangssteuerung {
  private:
    static const byte ZEILEN = 4;
    static const byte SPALTEN = 4;

    char tasten[ZEILEN][SPALTEN] = {
      {'1', '2', '3', 'A'},
      {'4', '5', '6', 'B'},
      {'7', '8', '9', 'C'},
      {'*', '0', '#', 'D'}
    };

    byte zeilenPins[ZEILEN] = {12, 14, 27, 26};
    byte spaltenPins[SPALTEN] = {25, 33, 32, 21};

    Keypad keypad;

    Tuersteuerung& tuer;
    Tuerkontakt& tuerKontakt;
    Statusanzeige& status;
    Displaysteuerung& display;

    String eingegebenePin;
    String richtigePin;
    Preferences preferences;

    unsigned long rueckmeldungSeit;
    unsigned long rueckmeldungDauer;
    bool rueckmeldungAktiv;

    int fehlversuche;
    bool gesperrt;
    unsigned long sperreSeit;
    unsigned long sperrDauer;
    unsigned long letzteAnzeigeSekunde;

    void tasteVerarbeiten(char taste);
    void pinPruefen();

    void rueckmeldungPruefen();
    void sperrePruefen();

    bool neuePinPruefen(
      String neuePin,
      String pinBestaetigung,
      String& fehlermeldung
    );

  public:
    Zugangssteuerung(
      Tuersteuerung& tuer,
      Tuerkontakt& tuerKontakt,
      Statusanzeige& status,
      Displaysteuerung& display
    );

    void pruefen();
    void begin();

    bool pinAendern(
      String altePin,
      String neuePin,
      String pinBestaetigung,
      String& fehlermeldung
    );

    bool pinZuruecksetzen(
      String neuePin,
      String pinBestaetigung,
      String& fehlermeldung
    );

    bool istGesperrt();
    int getFehlversuche();
    unsigned long getRestlicheSperrzeit();
};

#endif