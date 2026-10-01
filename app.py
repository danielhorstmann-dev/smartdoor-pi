from flask import Flask, render_template, jsonify
from mqtt_client import MqttClient


app = Flask(__name__)

mqtt = MqttClient()


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/api/status")
def status():
    return jsonify(
        mqtt.get_status()
    )


@app.route("/api/licht/an", methods=["POST"])
def licht_an():

    mqtt.publish(
        "smartdoor/licht/befehl",
        "an"
    )

    return "Licht AN gesendet"


@app.route("/api/licht/aus", methods=["POST"])
def licht_aus():

    mqtt.publish(
        "smartdoor/licht/befehl",
        "aus"
    )

    return "Licht AUS gesendet"

@app.route("/api/tuer/entriegeln", methods=["POST"])
def tuer_entriegeln():
    mqtt.publish(
        "smartdoor/tuer/befehl",
        "entriegeln"
    )

    return "Tuer entriegeln gesendet"

@app.route("/api/tuer/verriegeln", methods=["POST"])
def tuer_verriegeln():
    mqtt.publish(
        "smartdoor/tuer/befehl",
        "verriegeln"
    )

    return "Tuer verriegeln gesendet"

@app.route("/api/automatik/an", methods=["POST"])
def automatik_an():
    mqtt.publish(
        "smartdoor/automatik/befehl",
        "an"
    )

    return "Automatik AN gesendet"

@app.route("/api/automatik/aus", methods=["POST"])
def automatik_aus():
    mqtt.publish(
        "smartdoor/automatik/befehl",
        "aus"
    )

    return "Automatik AUS gesendet"


if __name__ == "__main__":

    mqtt.connect()

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=False
    )
