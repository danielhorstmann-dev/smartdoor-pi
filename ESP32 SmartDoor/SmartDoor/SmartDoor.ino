#include <WiFi.h>
#include <WebServer.h>

#include "Lichtsteuerung.h"
#include "Tuersteuerung.h"
#include "Zugangssteuerung.h"
#include "Statusanzeige.h"
#include "Displaysteuerung.h"
#include "Mqttsteuerung.h"

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
Tuersteuerung tuer(18);
Statusanzeige status(2, 12, 15);
Displaysteuerung display;
Zugangssteuerung zugang(tuer, status, display);
Mqttsteuerung mqtt("192.168.178.78", 1883, licht, tuer, zugang);

void setup() {
    Serial.begin(115200);
    
    delay(1000);
    Serial.println();
    Serial.println("===== ESP32 GESTARTET =====");

    licht.begin();
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
    tuer.pruefen();
    status.pruefen();
}