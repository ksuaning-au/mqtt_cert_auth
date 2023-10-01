import paho.mqtt.client as mqtt
import time


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + mqtt.error_string(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/test/#")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))


def main():
    client = mqtt.Client("myclient")
    client.tls_set(ca_certs="ca.crt", certfile="client.crt", keyfile="client.key")
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect("mqtt.mydomain.com", 8883, 60)

    # Create background thread for handling mqtt receiving
    client.loop_start()

    # Infinite Loop that publishes a message every 10 seconds.
    while True:
        client.publish("/test/python_client", "payload")
        time.sleep(10)


if __name__ == "__main__":
    main()
