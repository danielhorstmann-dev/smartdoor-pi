#ifndef LICHTSTEUERUNG_H
#define LICHTSTEUERUNG_H

#include <Arduino.h>

class Lichtsteuerung {

private:
    int lichtPin;
    int tasterPin;
    int ldrPin;
    int pirPin;

    int dunkelGrenze;

    int aktuelleHelligkeit;
    bool bewegungErkannt;
    bool automatikAktiv;

    bool lichtAn;
    bool lichtAutomatisch;
    bool istDunkel;

    bool tasterVorher;
    bool pirVorher;

    unsigned long letzteLdrMessung;

    void tasterPruefen();
    void helligkeitPruefen();
    void bewegungPruefen();

public:
    Lichtsteuerung(
        int lichtPin,
        int tasterPin,
        int ldrPin,
        int pirPin,
        int dunkelGrenze
    );

    void begin();
    void pruefen();

    void einschalten();
    void ausschalten();
    void manuellEinschalten();
    void manuellAusschalten();
    void umschalten();

    int getHelligkeit();
    bool istDunkelAktuell();
    bool istBewegungErkannt();

    bool istAutomatikAktiv();
    void automatikEinschalten();
    void automatikAusschalten();

    bool istAn();
};

#endif