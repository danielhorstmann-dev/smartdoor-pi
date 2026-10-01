#include "Zugangssteuerung.h"

Zugangssteuerung::Zugangssteuerung(
  Tuersteuerung& tuer,
  Tuerkontakt& tuerKontakt,
  Statusanzeige& status,
  Displaysteuerung& display
)
  : keypad(
      makeKeymap(tasten),
      zeilenPins,
      spaltenPins,
      ZEILEN,
      SPALTEN
    ),
    tuer(tuer),
    tuerKontakt(tuerKontakt),
    status(status),
    display(display)
{
  richtigePin = "1234";
  eingegebenePin = "";

  rueckmeldungSeit = 0;
  rueckmeldungDauer = 3000;
  rueckmeldungAktiv = false;

  fehlversuche = 0;
  gesperrt = false;

  sperreSeit = 0;
  sperrDauer = 30000;
  letzteAnzeigeSekunde = 0;
}

void Zugangssteuerung::rueckmeldungPruefen() {

  if (rueckmeldungAktiv &&
      millis() - rueckmeldungSeit >= rueckmeldungDauer) {

    display.bereit();
    status.bereit();

    rueckmeldungAktiv = false;
  }
}

void Zugangssteuerung::pruefen() {

  sperrePruefen();

  if (gesperrt) {
    return;
  }

  rueckmeldungPruefen();

  // Während der 3 Sekunden Rückmeldung
  // keine neue PIN-Eingabe verarbeiten
  if (rueckmeldungAktiv) {
    return;
  }

  char taste = keypad.getKey();

  if (taste) {
    tasteVerarbeiten(taste);
  }
}

void Zugangssteuerung::begin() {

  preferences.begin("smartdoor", false);

  richtigePin = preferences.getString("pin", "1234");

  Serial.println("Zugangssteuerung gestartet");
  Serial.println("PIN aus Speicher geladen");
}

bool Zugangssteuerung::pinAendern(
  String altePin,
  String neuePin,
  String pinBestaetigung,
  String& fehlermeldung
) {

  if (altePin != richtigePin) {
    fehlermeldung = "Aktuelle PIN ist falsch";
    return false;
  }

  if (!neuePinPruefen(
        neuePin,
        pinBestaetigung,
        fehlermeldung
      )) {
    return false;
  }

  richtigePin = neuePin;

  preferences.putString("pin", richtigePin);

  fehlermeldung = "PIN erfolgreich geaendert";

  Serial.println("PIN wurde erfolgreich geaendert");

  return true;
}

bool Zugangssteuerung::neuePinPruefen(
  String neuePin,
  String pinBestaetigung,
  String& fehlermeldung
) {

  if (neuePin.length() < 4 ||
      neuePin.length() > 8) {

    fehlermeldung =
      "Neue PIN muss 4 bis 8 Ziffern haben";

    return false;
  }

  for (int i = 0; i < neuePin.length(); i++) {

    if (!isDigit(neuePin[i])) {

      fehlermeldung =
        "PIN darf nur Zahlen enthalten";

      return false;
    }
  }

  if (neuePin != pinBestaetigung) {

    fehlermeldung =
      "Neue PINs stimmen nicht ueberein";

    return false;
  }

  return true;
}

bool Zugangssteuerung::pinZuruecksetzen(
  String neuePin,
  String pinBestaetigung,
  String& fehlermeldung
) {

  if (!neuePinPruefen(
        neuePin,
        pinBestaetigung,
        fehlermeldung
      )) {
    return false;
  }

  richtigePin = neuePin;

  preferences.putString(
    "pin",
    richtigePin
  );

  fehlermeldung =
    "PIN erfolgreich zurueckgesetzt";

  Serial.println(
    "PIN wurde ueber Web-Admin zurueckgesetzt"
  );

  return true;
}

void Zugangssteuerung::tasteVerarbeiten(char taste) {

  if (taste >= '0' && taste <= '9') {

    eingegebenePin += taste;

    display.pinEingabe(eingegebenePin.length());

    Serial.print("PIN: ");

    for (int i = 0; i < eingegebenePin.length(); i++) {
      Serial.print("*");
    }

    Serial.println();

  } else if (taste == '*') {

    eingegebenePin = "";

    display.bereit();

    Serial.println("PIN-Eingabe geloescht.");

  } else if (taste == '#') {

    pinPruefen();
  }
}

void Zugangssteuerung::pinPruefen() {

if (eingegebenePin == richtigePin) {

    Serial.println("PIN korrekt - Zugang erlaubt");

    fehlversuche = 0;

    // Nur entriegeln, wenn aktuell verriegelt
    if (!tuer.istEntriegelt()) {

        tuer.entriegeln();

        status.erfolg();
        display.zugangErlaubt();

    } else {

        Serial.println("Tuer ist bereits entriegelt");

        status.erfolg();
        display.zugangErlaubt();
    }
  } else {

    fehlversuche++;

    Serial.println("PIN falsch - Fehlversuch:");
    Serial.println(fehlversuche);

    if (fehlversuche >= 3) {

      gesperrt = true;
      sperreSeit = millis();

      status.gesperrt();
      display.gesperrt(30);

      Serial.println(
        "Zugang fuer 30 Sekunden gesperrt"
      );

    } else {

      status.fehler();
      display.zugangVerweigert();
    }
  }

  // PIN-Puffer nach jeder Prüfung löschen
  eingegebenePin = "";

  // Drei Sekunden Rückmeldung anzeigen
  rueckmeldungSeit = millis();
  rueckmeldungAktiv = true;
}

void Zugangssteuerung::sperrePruefen() {

  if (!gesperrt) {
    return;
  }

  unsigned long restzeit =
    getRestlicheSperrzeit();

  if (restzeit != letzteAnzeigeSekunde) {

    letzteAnzeigeSekunde = restzeit;

    display.gesperrt(restzeit);
  }

  if (millis() - sperreSeit >= sperrDauer) {

    gesperrt = false;
    fehlversuche = 0;
    rueckmeldungAktiv = false;
    letzteAnzeigeSekunde = 0;

    status.bereit();
    display.bereit();

    Serial.println(
      "Zugangssperre aufgehoben"
    );
  }
}

bool Zugangssteuerung::istGesperrt() {
  return gesperrt;
}

int Zugangssteuerung::getFehlversuche() {
  return fehlversuche;
}

unsigned long Zugangssteuerung::getRestlicheSperrzeit() {

  if (!gesperrt) {
    return 0;
  }

  unsigned long vergangen =
    millis() - sperreSeit;

  if (vergangen >= sperrDauer) {
    return 0;
  }

  return (sperrDauer - vergangen + 999) / 1000;
}