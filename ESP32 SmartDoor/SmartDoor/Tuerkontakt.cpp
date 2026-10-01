#include "Tuerkontakt.h"

Tuerkontakt::Tuerkontakt(int kontaktPin) {
  this->kontaktPin = kontaktPin;
  tuerOffen = false;
}

void Tuerkontakt::begin() {

  pinMode(kontaktPin, INPUT_PULLUP);

  pruefen();
}

void Tuerkontakt::pruefen() {

  bool kontakt = digitalRead(kontaktPin);

  if (kontakt == LOW) {
    tuerOffen = false;
  } else {
    tuerOffen = true;
  }
}

bool Tuerkontakt::istOffen() {
  return tuerOffen;
}

bool Tuerkontakt::istGeschlossen() {
  return !tuerOffen;
}