from flask import (
    Flask, 
    render_template, 
    jsonify, 
    request,
    Response
)
from mqtt_client import MqttClient


app = Flask(__name__)

mqtt = MqttClient()
WEB_BENUTZER = "admin"
WEB_PASSWORT = "smartdoor"

def authentifiziert():

    auth = request.authorization

    return (
        auth is not None
        and auth.username == WEB_BENUTZER
        and auth.password == WEB_PASSWORT
    )


def anmeldung_erforderlich():

    return Response(
        "Anmeldung erforderlich",
        401,
        {
            "WWW-Authenticate":
            'Basic realm="SmartDoor"'
        }
    )


@app.route("/")
def index():

    if not authentifiziert():
        return anmeldung_erforderlich()
    
    return render_template("index.html")


@app.route("/api/status")
def status():
    return jsonify(
        mqtt.get_status()
    )


@app.route("/api/licht/an", methods=["POST"])
def licht_an():

    if not authentifiziert():
        return anmeldung_erforderlich()

    mqtt.publish(
        "smartdoor/licht/befehl",
        "an"
    )

    return "Licht AN gesendet"


@app.route("/api/licht/aus", methods=["POST"])
def licht_aus():
    
    if not authentifiziert():
        return anmeldung_erforderlich()

    mqtt.publish(
        "smartdoor/licht/befehl",
        "aus"
    )

    return "Licht AUS gesendet"

@app.route("/api/tuer/entriegeln", methods=["POST"])
def tuer_entriegeln():
    
    if not authentifiziert():
        return anmeldung_erforderlich()

    mqtt.publish(
        "smartdoor/tuer/befehl",
        "entriegeln"
    )

    return "Tuer entriegeln gesendet"

@app.route("/api/tuer/verriegeln", methods=["POST"])
def tuer_verriegeln():
    
    if not authentifiziert():
        return anmeldung_erforderlich()

    mqtt.publish(
        "smartdoor/tuer/befehl",
        "verriegeln"
    )

    return "Tuer verriegeln gesendet"

@app.route("/api/automatik/an", methods=["POST"])
def automatik_an():
    
    if not authentifiziert():
        return anmeldung_erforderlich()

    mqtt.publish(
        "smartdoor/automatik/befehl",
        "an"
    )

    return "Automatik AN gesendet"

@app.route("/api/automatik/aus", methods=["POST"])
def automatik_aus():
    
    if not authentifiziert():
        return anmeldung_erforderlich()

    mqtt.publish(
        "smartdoor/automatik/befehl",
        "aus"
    )

    return "Automatik AUS gesendet"

@app.route("/api/pin/aendern", methods=["POST"])
def pin_aendern():
    
    if not authentifiziert():
        return anmeldung_erforderlich()


    alte_pin = request.form.get("altePin")
    neue_pin = request.form.get("neuePin")
    bestaetigung = request.form.get(
        "pinBestaetigung"
    )

    nachricht = (
        f"{alte_pin};"
        f"{neue_pin};"
        f"{bestaetigung}"
    )

    mqtt.publish(
        "smartdoor/pin/aendern",
        nachricht
    )

    return "PIN-Aenderung gesendet"


@app.route("/api/pin/reset", methods=["POST"])
def pin_reset():
    
    if not authentifiziert():
        return anmeldung_erforderlich()
        

    neue_pin = request.form.get("neuePin")
    bestaetigung = request.form.get(
        "pinBestaetigung"
    )

    nachricht = (
        f"{neue_pin};"
        f"{bestaetigung}"
    )

    mqtt.publish(
        "smartdoor/pin/reset",
        nachricht
    )

    return "PIN-Reset gesendet"


if __name__ == "__main__":

    mqtt.connect()

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=False
    )
