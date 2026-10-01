#ifndef MQTTSTEUERUNG_H
#define MQTTSTEUERUNG_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "Lichtsteuerung.h"
#include "Tuersteuerung.h"
#include "Tuerkontakt.h"
#include "Zugangssteuerung.h"
#include "Statusanzeige.h"
#include "Displaysteuerung.h"

class Mqttsteuerung {

private:
    const char* broker;
    int port;

    unsigned long letzterVerbindungsversuch;
    unsigned long reconnectIntervall;

    Lichtsteuerung& licht;
    Tuersteuerung& tuer;
    Tuerkontakt& tuerKontakt;
    Zugangssteuerung& zugang;
    Statusanzeige& status;
    Displaysteuerung& display;

    static Mqttsteuerung* instanz;

    unsigned long letzterStatus;
    unsigned long statusIntervall;

    void statusSenden();

    WiFiClient wifiClient;
    PubSubClient mqttClient;

    void verbinden();

    void nachrichtEmpfangen(
        char* topic,
        byte* payload,
        unsigned int length
    );

    static void callback(
        char* topic,
        byte* payload,
        unsigned int length
    );

public:
    Mqttsteuerung(
        const char* broker,
        int port,
        Lichtsteuerung& licht,
        Tuersteuerung& tuer,
        Tuerkontakt& tuerKontakt,
        Zugangssteuerung& zugang,
        Statusanzeige& status,
        Displaysteuerung& display
    );

    void begin();
    void pruefen();

    void senden(
        const char* topic,
        const char* nachricht
    );
};

#endif