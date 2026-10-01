#include "Tuersteuerung.h"

Tuersteuerung::Tuersteuerung(int servoPin, Tuerkontakt& tuerKontakt)
  : tuerKontakt(tuerKontakt)
{
  this->servoPin = servoPin;
  tuerEntriegelt = false;
  tuerWurdeGeoeffnet = false;
  entriegeltSeit = 0;
  oeffnungsTimeout = 10000;
}

void Tuersteuerung::begin() {
  servo.attach(servoPin);

  verriegeln();
}

void Tuersteuerung::pruefen() {

  if (!tuerEntriegelt) {
    return;
  }

  if (!tuerWurdeGeoeffnet) {
    if (tuerKontakt.istOffen()) {
      tuerWurdeGeoeffnet = true;

      Serial.println("Tuer wurde geoeffnet");

      return;
    }

    if (millis() - entriegeltSeit >= oeffnungsTimeout) {
          
      Serial.println("Tuer wurde nicht geoeffnet");
      Serial.println("Automatische Verriegelung");  

      verriegeln();
    }

    return;
  }

  if (tuerKontakt.istGeschlossen()) {
    Serial.println("Tuer wurde geschlossen");
    Serial.println("Automatische Verriegelung");

    verriegeln();
  }

}

void Tuersteuerung::entriegeln() {
  servo.write(90);
  tuerEntriegelt = true;
  tuerWurdeGeoeffnet = false;

  entriegeltSeit = millis();

  Serial.println("Tuer ENTRIEGELT");
}

void Tuersteuerung::verriegeln() {
  servo.write(0);
  tuerEntriegelt = false;
  tuerWurdeGeoeffnet = false;

  Serial.println("Tuer VERRIEGELT");
}

bool Tuersteuerung::istEntriegelt() {
  return tuerEntriegelt;
}
