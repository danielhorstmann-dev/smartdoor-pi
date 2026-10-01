#include "Tuersteuerung.h"

Tuersteuerung::Tuersteuerung(int servoPin) {
  this->servoPin = servoPin;
  tuerEntriegelt = false;
  entriegeltSeit = 0;
  offenDauer = 5000;
  automatischVerriegeln = false;
}

void Tuersteuerung::begin() {
  servo.attach(servoPin);

  verriegeln();
}

void Tuersteuerung::pruefen() {
  if (automatischVerriegeln &&
      millis() - entriegeltSeit >= offenDauer) {
        
    Serial.println("Tuer automatisch verriegelt");  

    verriegeln();
  }
}

void Tuersteuerung::entriegeln() {
  servo.write(90);
  tuerEntriegelt = true;

  entriegeltSeit = millis();
  automatischVerriegeln = true;

  Serial.println("Tuer ENTRIEGELT");
}

void Tuersteuerung::verriegeln() {
  servo.write(0);
  tuerEntriegelt = false;

  automatischVerriegeln = false;

  Serial.println("Tuer VERRIEGELT");
}

bool Tuersteuerung::istEntriegelt() {
  return tuerEntriegelt;
}
