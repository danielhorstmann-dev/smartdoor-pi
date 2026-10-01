#include "Displaysteuerung.h"

Displaysteuerung::Displaysteuerung()
  : lcd(0x27, 16, 2)
{

}

void Displaysteuerung::begin() {

  Wire.begin(21,22);

  lcd.init();
  lcd.backlight();

  bereit();
}

void Displaysteuerung::leeren() {
  lcd.clear();
}

void Displaysteuerung::bereit() {

  leeren();

  lcd.setCursor(0,0);
  lcd.print("Smart Door");

  lcd.setCursor(0,1);
  lcd.print("PIN eingeben");
}

void Displaysteuerung::gesperrt(unsigned long sekunden) {

  leeren();

  lcd.setCursor(0,0);
  lcd.print("Zugang gesperrt");

  lcd.setCursor(0,1);
  lcd.print("Noch ");

  lcd.print(sekunden);

  lcd.print(" Sek.    ");
}

void Displaysteuerung::pinEingabe(int anzahl) {

  lcd.setCursor(0,1);
  lcd.print("PIN:          ");

  lcd.setCursor(5,1);

  for (int i = 0; i < anzahl; i++) {
    lcd.print("*");
  }
}

void Displaysteuerung::zugangErlaubt() {

  leeren();

  lcd.setCursor(0,0);
  lcd.print("Zugang erlaubt");

  lcd.setCursor(0,1);
  lcd.print("Entriegelt");
}

void Displaysteuerung::zugangVerweigert() {

  leeren();

  lcd.setCursor(0,0);
  lcd.print("PIN falsch!");

  lcd.setCursor(0,1);
  lcd.print("Kein Zugang");
}