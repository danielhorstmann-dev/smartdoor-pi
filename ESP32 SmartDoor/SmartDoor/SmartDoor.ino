#include <WiFi.h>
#include <WebServer.h>

#include "Lichtsteuerung.h"
#include "Tuersteuerung.h"
#include "Zugangssteuerung.h"
#include "Statusanzeige.h"
#include "Displaysteuerung.h"
#include "Mqttsteuerung.h"
#include "Tuerkontakt.h"

const char* ssid = "FRITZ!Box 6670 II 2,4 GHz";
const char* passwort = "42855128522883691014";

void wlanVerbinden() {

    WiFi.begin(
        ssid,
        passwort
    );

    Serial.print("Verbinde mit WLAN");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WLAN verbunden");

    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
}

WebServer server(80);

Lichtsteuerung licht(23, 19, 34, 27, 2200);
Tuerkontakt tuerKontakt(4);
Tuersteuerung tuer(18, tuerKontakt);
Statusanzeige status(2, 12, 15);
Displaysteuerung display;
Zugangssteuerung zugang(tuer, tuerKontakt, status, display);
Mqttsteuerung mqtt("192.168.178.78", 1883, licht, tuer, tuerKontakt, zugang, status, display);

bool tuerWarOffen = false;

bool tuerMeldungAktiv = false;
unsigned long tuerMeldungSeit = 0;
const unsigned long tuerMeldungDauer = 2000;

bool timeoutMeldungAktiv = false;
unsigned long timeoutMeldungSeit = 0;
const unsigned long timeoutMeldungDauer = 3000;

void timeoutMeldungPruefen() {

    if (tuer.wurdeTimeoutAusgeloest()) {

        display.tuerNichtGeoeffnet();

        timeoutMeldungSeit = millis();
        timeoutMeldungAktiv = true;
    }


    if (
        timeoutMeldungAktiv &&
        millis() - timeoutMeldungSeit >= timeoutMeldungDauer
    ) {

        display.bereit();

        timeoutMeldungAktiv = false;
    }
}

void tuerstatusPruefen() {

    bool tuerIstOffen =
        tuerKontakt.istOffen();


    // -----------------------------
    // Zustandsänderung erkennen
    // -----------------------------

    if (tuerIstOffen != tuerWarOffen) {

        tuerWarOffen =
            tuerIstOffen;


        if (tuerIstOffen) {

            Serial.println(
                "Tuerstatus: OFFEN"
            );

            display.tuerGeoeffnet();

        } else {

            Serial.println(
                "Tuerstatus: GESCHLOSSEN"
            );

            display.tuerGeschlossen();
        }


        tuerMeldungSeit =
            millis();

        tuerMeldungAktiv =
            true;
    }


    // -----------------------------
    // Nach 2 Sekunden zurück
    // auf Bereitschaft
    // -----------------------------

    if (
        tuerMeldungAktiv &&
        millis() - tuerMeldungSeit
            >= tuerMeldungDauer
    ) {

        display.bereit();

        tuerMeldungAktiv =
            false;
    }
}

void setup() {
    Serial.begin(115200);
    
    delay(1000);
    Serial.println();
    Serial.println("===== ESP32 GESTARTET =====");

    licht.begin();
    tuerKontakt.begin();
    tuerWarOffen = tuerKontakt.istOffen();
    tuer.begin();
    status.begin();
    display.begin();
    zugang.begin();
    wlanVerbinden();
    mqtt.begin();
}

void loop() {
    mqtt.pruefen();
    
    licht.pruefen();
    zugang.pruefen();
    tuerKontakt.pruefen();
    tuerstatusPruefen();
    tuer.pruefen();
    timeoutMeldungPruefen();
    status.pruefen();
}