#include "Mqttsteuerung.h"

Mqttsteuerung* Mqttsteuerung::instanz = nullptr;


Mqttsteuerung::Mqttsteuerung(
    const char* broker,
    int port,
    Lichtsteuerung& licht,
    Tuersteuerung& tuer,
    Tuerkontakt& tuerKontakt,
    Zugangssteuerung& zugang,
    Statusanzeige& status,
    Displaysteuerung& display
)
    : broker(broker),
      port(port),
      mqttClient(wifiClient),
      licht(licht),
      tuer(tuer),
      tuerKontakt(tuerKontakt),
      zugang(zugang),
      status(status),
      display(display)
{
    letzterVerbindungsversuch = 0;
    reconnectIntervall = 5000;

    letzterStatus = 0;
    statusIntervall = 2000;

    instanz = this;
}


void Mqttsteuerung::begin() {

    mqttClient.setServer(
        broker,
        port
    );

    mqttClient.setCallback(
        callback
    );

    verbinden();
}


void Mqttsteuerung::callback(
    char* topic,
    byte* payload,
    unsigned int length
) {

    if (instanz != nullptr) {

        instanz->nachrichtEmpfangen(
            topic,
            payload,
            length
        );
    }
}


void Mqttsteuerung::nachrichtEmpfangen(
    char* topic,
    byte* payload,
    unsigned int length
) {

    String nachricht = "";

    for (unsigned int i = 0; i < length; i++) {

        nachricht +=
            (char)payload[i];
    }


    Serial.print("MQTT empfangen: ");
    Serial.print(topic);
    Serial.print(" -> ");
    Serial.println(nachricht);


    if (
        String(topic) ==
        "smartdoor/licht/befehl"
    ) {

        if (nachricht == "an") {

            licht.manuellEinschalten();
        }

        else if (nachricht == "aus") {

            licht.manuellAusschalten();
        }
    }

    else if (
        String(topic) ==
        "smartdoor/tuer/befehl"
    ) {

        if (nachricht == "entriegeln") {
            tuer.entriegeln();

            status.erfolg();
            display.zugangErlaubt();
        }

        if (nachricht == "verriegeln") {
            tuer.verriegeln();
        }
    }

    else if (
        String(topic) ==
        "smartdoor/automatik/befehl"
    ) {

        if (nachricht == "an") {
            licht.automatikEinschalten();
        }

        if (nachricht == "aus") {
            licht.automatikAusschalten();
        }
    }

    else if (
        String(topic) ==
        "smartdoor/pin/aendern"
    ) {

        int trennstelle1 =
            nachricht.indexOf(';');

        int trennstelle2 =
            nachricht.indexOf(
                ';',
                trennstelle1 + 1
            );

        if (
            trennstelle1 == -1 ||
            trennstelle2 == -1
        ) {

            Serial.println(
                "PIN-Aenderung: Ungueltige Daten"
            );

            return;
        }

        String altePin =
            nachricht.substring(
                0,
                trennstelle1
            );

        String neuePin =
            nachricht.substring(
                trennstelle1 + 1,
                trennstelle2
            );

        String bestaetigung =
            nachricht.substring(
                trennstelle2 + 1
            );

        String meldung;

        zugang.pinAendern(
            altePin,
            neuePin,
            bestaetigung,
            meldung
        );

        Serial.println(meldung);
    }

    else if (
        String(topic) ==
        "smartdoor/pin/reset"
    ) {

        int trennstelle =
            nachricht.indexOf(';');

        if (trennstelle == -1) {

            Serial.println(
                "PIN-Reset: Ungueltige Daten"
            );

            return;
        }

        String neuePin =
            nachricht.substring(
                0,
                trennstelle
            );

        String bestaetigung =
            nachricht.substring(
                trennstelle + 1
            );

        String meldung;

        zugang.pinZuruecksetzen(
            neuePin,
            bestaetigung,
            meldung
        );

        Serial.println(meldung);
    }
}


void Mqttsteuerung::verbinden() {

    Serial.print(
        "Verbinde mit MQTT-Broker..."
    );

    if (
        mqttClient.connect(
            "SmartDoorESP32"
        )
    ) {

        Serial.println(" erfolgreich");


        mqttClient.subscribe(
            "smartdoor/licht/befehl"
        );

        mqttClient.subscribe(
            "smartdoor/tuer/befehl"
        );

        mqttClient.subscribe(
            "smartdoor/automatik/befehl"
        );

        mqttClient.subscribe(
            "smartdoor/pin/aendern"
        );

        mqttClient.subscribe(
            "smartdoor/pin/reset"
        );


        senden(
            "smartdoor/system/status",
            "online"
        );

        // Nach Verbindungsaufbau direkt
        // den aktuellen Zustand übertragen
        statusSenden();

    } else {

        Serial.print(
            " FEHLER, Status: "
        );

        Serial.println(
            mqttClient.state()
        );
    }
}


void Mqttsteuerung::pruefen() {

    if (!mqttClient.connected()) {

        if (
            millis() -
            letzterVerbindungsversuch
            >= reconnectIntervall
        ) {

            letzterVerbindungsversuch =
                millis();

            verbinden();
        }

        return;
    }


    mqttClient.loop();


    if (
        millis() -
        letzterStatus
        >= statusIntervall
    ) {

        letzterStatus =
            millis();

        statusSenden();
    }
}


void Mqttsteuerung::senden(
    const char* topic,
    const char* nachricht
) {

    if (!mqttClient.connected()) {
        return;
    }

    mqttClient.publish(
        topic,
        nachricht
    );
}


void Mqttsteuerung::statusSenden() {

    senden(
        "smartdoor/licht/status",
        licht.istAn()
            ? "an"
            : "aus"
    );


    senden(
        "smartdoor/schloss/status",
        tuer.istEntriegelt()
            ? "entriegelt"
            : "verriegelt"
    );


    // NEU:
    // Tatsächlicher Zustand der Tür
    senden(
        "smartdoor/tuer/status",
        tuerKontakt.istOffen()
            ? "offen"
            : "geschlossen"
    );


    senden(
        "smartdoor/bewegung",
        licht.istBewegungErkannt()
            ? "erkannt"
            : "keine"
    );


    String helligkeit =
        String(
            licht.getHelligkeit()
        );

    senden(
        "smartdoor/helligkeit",
        helligkeit.c_str()
    );
}