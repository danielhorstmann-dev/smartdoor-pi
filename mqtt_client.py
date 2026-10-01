import paho.mqtt.client as mqtt


class MqttClient:

    def __init__(self, broker="localhost", port=1883):
        self.broker = broker
        self.port = port

        self.status = {
            "licht": "unbekannt",
            "schloss": "unbekannt",
            "tuer": "unbekannt",
            "bewegung": "unbekannt",
            "helligkeit": 0
        }

        self.client = mqtt.Client(
            mqtt.CallbackAPIVersion.VERSION2
        )

        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message


    def connect(self):
        print(
            f"Verbinde mit MQTT-Broker "
            f"{self.broker}:{self.port}"
        )

        self.client.connect(
            self.broker,
            self.port,
            60
        )

        self.client.loop_start()


    def on_connect(
        self,
        client,
        userdata,
        flags,
        reason_code,
        properties
    ):
        print(
            f"MQTT verbunden: {reason_code}"
        )

        client.subscribe(
            "smartdoor/#"
        )


    def on_message(
        self,
        client,
        userdata,
        message
    ):
        payload = message.payload.decode()

        print(
            f"MQTT: {message.topic} -> {payload}"
        )

        if message.topic == "smartdoor/licht/status":
            self.status["licht"] = payload

        elif message.topic == "smartdoor/schloss/status":
            self.status["schloss"] = payload

        elif message.topic == "smartdoor/tuer/status":
            self.status["tuer"] = payload

        elif message.topic == "smartdoor/bewegung":
            self.status["bewegung"] = payload

        elif message.topic == "smartdoor/helligkeit":
            self.status["helligkeit"] = int(payload)


    def get_status(self):
        return self.status


    def publish(self, topic, message):
        self.client.publish(
            topic,
            message
        )
