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


if __name__ == "__main__":

    mqtt.connect()

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=False
    )
