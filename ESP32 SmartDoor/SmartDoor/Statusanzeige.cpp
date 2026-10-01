#include "Statusanzeige.h"

Statusanzeige::Statusanzeige(int rotPin, int gruenPin, int blauPin) {
  this->rotPin = rotPin;
  this->gruenPin = gruenPin;
  this->blauPin = blauPin;

  statusSeit = 0;
  statusDauer = 3000;
  statusZeitAktiv = false;
  //this->buzzerPin = buzzerPin;

  //buzzerAktiv = false;
  //buzzerBis = 0;
}

void Statusanzeige::begin() {
  pinMode(rotPin, OUTPUT);
  pinMode(gruenPin, OUTPUT);
  pinMode(blauPin, OUTPUT);
  //pinMode(buzzerPin, OUTPUT);
  //digitalWrite(buzzerPin, HIGH);

  //aus();
  bereit();
}

void Statusanzeige::farbeSetzen(bool rot, bool gruen, bool blau) {

  digitalWrite(rotPin, rot ? LOW : HIGH);
  digitalWrite(gruenPin, gruen ? LOW : HIGH);
  digitalWrite(blauPin, blau ? LOW : HIGH);
}

void Statusanzeige::aus() {
  farbeSetzen(false, false, false);
}

void Statusanzeige::rot() {
  farbeSetzen(true, false, false);
}

void Statusanzeige::gruen() {
  farbeSetzen(false, true, false);
}

void Statusanzeige::blau() {
  farbeSetzen(false, false, true);
}

//void Statusanzeige::buzzerStarten(unsigned long dauer) {
//  digitalWrite(buzzerPin, LOW);

//  buzzerAktiv = true;
//  buzzerBis = millis() + dauer;
//}

//void Statusanzeige::pruefen() {
//  if (buzzerAktiv && millis() >= buzzerBis) {
//    digitalWrite(buzzerPin, HIGH);
//    buzzerAktiv = false;
//  }
//}

void Statusanzeige::pruefen() {
  if (statusZeitAktiv &&
      millis() - statusSeit >= statusDauer) {
        bereit();
      }
}

void Statusanzeige::bereit() {
  blau();
}

void Statusanzeige::erfolg() {
  gruen();

  statusSeit = millis();
  statusZeitAktiv = true;
//  buzzerStarten(150);
}

void Statusanzeige::fehler() {
  rot();

  statusSeit = millis();
  statusZeitAktiv = true;
//  buzzerStarten(500);
}

void Statusanzeige::gesperrt() {
  rot();

  statusZeitAktiv = false;
}