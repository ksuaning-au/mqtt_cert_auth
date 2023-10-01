#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>
#include <WiFiClientSecure.h>

const char ssid[] = "YOURSSID";
const char pass[] = "YOURPASSWORD";

const char* serverCA = R"EOF(-----BEGIN CERTIFICATE-----
YOUR CA CERT
-----END CERTIFICATE-----)EOF";

const char* clientCert = R"KEY(-----BEGIN CERTIFICATE-----
YOUR CLIENT CERT
-----END CERTIFICATE-----)KEY";

const char* clientKey = R"KEY(-----BEGIN RSA PRIVATE KEY-----
YOUR CLIENT KEY
-----END RSA PRIVATE KEY-----)KEY";

WiFiClientSecure net;
MQTTClient client;

unsigned long lastMillis = 0;

void messageReceived(String &topic, String &payload) {
  /**
   * MQTT Client message recived callback.
   */
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);

  // Connect to the WiFi
   WiFi.begin(ssid, pass);

  Serial.print("Checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
	Serial.println("WiFi connected!");

  // Configure WiFiClientSecure with the required certs
  net.setCACert(serverCA);
	net.setCertificate(clientCert);
	net.setPrivateKey(clientKey);

  // Set a timeout for our WiFiClient so it doesn't hang on disconnect
  net.setTimeout(5);
  
  // Start the mqtt client
  client.begin("mqtt.mydomain.com", 8883, net);
  
  // Set the mqtt client message callback
	client.onMessage(messageReceived);

  // Connect to the mqtt broker
  while (!client.connect("myclient", "public", "public")) {
		Serial.print(".");
		delay(1000);
	}

	Serial.println("connected!");

  // Subscribe to the relevant topics
	client.subscribe("/test");
}

void loop() {
  // Run the mqtt client loop (handles recived messages - runs keep alive)
  client.loop();

  // Check that client is still connected - if not reconnect
  if (!client.connected()) {
    while (!client.connect("myclient", "public", "public")) {
      Serial.print(".");
      delay(1000);
	  }
    Serial.println("Reconnected to MQTT Broker!");
  }

  // Publish a message to a topic every second
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    client.publish("/test", "hello world");
  }
}