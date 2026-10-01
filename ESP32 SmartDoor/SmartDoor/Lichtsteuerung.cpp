#include "Lichtsteuerung.h"

Lichtsteuerung::Lichtsteuerung(
    int lichtPin,
    int tasterPin,
    int ldrPin,
    int pirPin,
    int dunkelGrenze
) {
    this->lichtPin = lichtPin;
    this->tasterPin = tasterPin;
    this->ldrPin = ldrPin;
    this->pirPin = pirPin;
    this->dunkelGrenze = dunkelGrenze;

    lichtAn = false;
    lichtAutomatisch = false;
    istDunkel = false;

    aktuelleHelligkeit = 0;
    bewegungErkannt = false;
    automatikAktiv = true;

    tasterVorher = HIGH;
    pirVorher = LOW;

    letzteLdrMessung = 0;
}

void Lichtsteuerung::begin() {
    pinMode(lichtPin, OUTPUT);
    pinMode(tasterPin, INPUT_PULLUP);
    pinMode(ldrPin, INPUT);
    pinMode(pirPin, INPUT);

    ausschalten();
}

void Lichtsteuerung::pruefen() {
    helligkeitPruefen();
    bewegungPruefen();
    tasterPruefen();
}

void Lichtsteuerung::helligkeitPruefen() {
    if (millis() - letzteLdrMessung >= 500) {
        aktuelleHelligkeit = analogRead(ldrPin);

        istDunkel = aktuelleHelligkeit < dunkelGrenze;

        letzteLdrMessung = millis();
    }
}

void Lichtsteuerung::bewegungPruefen() {
    bool bewegung = digitalRead(pirPin);
    bewegungErkannt = bewegung;

    if (automatikAktiv && 
            pirVorher == LOW && 
            bewegung == HIGH && 
            istDunkel && 
            !lichtAn) {
        einschalten();
        lichtAutomatisch = true;

        Serial.println("AUTOMATIK: Bewegung + dunkel -> Licht AN");
    }

    if (automatikAktiv && bewegung == LOW && lichtAutomatisch) {
        ausschalten();
        lichtAutomatisch = false;

        Serial.println("AUTOMATIK: Keine Bewegung mehr -> Licht AUS");
    }

    pirVorher = bewegung;
}

void Lichtsteuerung::tasterPruefen() {
    bool tasterJetzt = digitalRead(tasterPin);

    if (tasterVorher == HIGH && tasterJetzt == LOW) {

        lichtAutomatisch = false;

        umschalten();

        if (lichtAn) {
            Serial.println("Licht per Taster AN");
        } else {
            Serial.println("Licht per Taster AUS");
        }
    }

    tasterVorher = tasterJetzt;
}

void Lichtsteuerung::einschalten() {
    lichtAn = true;
    digitalWrite(lichtPin, HIGH);
}

void Lichtsteuerung::ausschalten() {
    lichtAn = false;
    digitalWrite(lichtPin, LOW);
}

void Lichtsteuerung::manuellEinschalten() {
    lichtAutomatisch = false;
    einschalten();
}

void Lichtsteuerung::manuellAusschalten() {
    lichtAutomatisch = false;
    ausschalten();
}

void Lichtsteuerung::umschalten() {
    if (lichtAn) {
        ausschalten();
    } else {
        einschalten();
    }
}

bool Lichtsteuerung::istAn() {
    return lichtAn;
}

int Lichtsteuerung::getHelligkeit() {
    return aktuelleHelligkeit;
}

bool Lichtsteuerung::istDunkelAktuell() {
    return istDunkel;
}

bool Lichtsteuerung::istBewegungErkannt() {
    return bewegungErkannt;
}

bool Lichtsteuerung::istAutomatikAktiv() {
    return automatikAktiv;
}

void Lichtsteuerung::automatikEinschalten() {
    automatikAktiv = true;

    Serial.println("Lichtautomatik aktiviert");
}

void Lichtsteuerung::automatikAusschalten() {
    automatikAktiv = false;
    lichtAutomatisch = false;

    Serial.println("Lichtautomatik deaktiviert");
}