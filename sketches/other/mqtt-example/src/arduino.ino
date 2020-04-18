
/*
	Test project for use with MQTT.
*/

// Include libraries.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// And the wifi configuration file.
#include "wifi.h"

long timeBetweenMessages = 1000 * 20 * 1;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;

int status = WL_IDLE_STATUS;

void setup_wifi() {
	delay(10);
	// We start by connecting to a wifi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();

	// Switch on the LED if a 1 was received as the first character.
	if ((char)payload[0] == '1') {
		digitalWrite(LED_BUILTIN, LOW);
	} else {
		digitalWrite(LED_BUILTIN, HIGH);
	}
}

String macToStr(const uint8_t* mac) {
	String result;
	for (int i = 0; i < 6; i++) {
		result += String(mac[i], 16);
		if (i < 5)
			result += ":";
	}
	return result;
}

String composeClientID() {
	uint8_t mac[6];
	WiFi.macAddress(mac);
	String clientId;
	clientId += "esp-";
	clientId += macToStr(mac);
	return clientId;
}

void reconnect() {
	// Loop until we're reconnected.
	while (!client.connected()) {
		Serial.print("Attemtping MQTT connection...");
		
		String clientId = composeClientID();
		clientId += "-";
		clientId += String(micros() & 0xff, 16); // to randomise, sort of // ???

		// Attempt to connect.
		if (client.connect(clientId.c_str())) {
			Serial.println("Connected");
			// Once connected, publish an announcement.
			client.publish(topic, ("connected " + composeClientID()).c_str(), true);
			// And resubscribe topic + clientID + in
			String subscription;
			subscription += topic;
			subscription += "/";
			subscription += composeClientID();
			subscription += "/in";
			client.subscribe(subscription.c_str());
			Serial.print("Subscribed to: ");
			Serial.println(subscription);
		} else {
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.print(" wifi=");
			Serial.print(WiFi.status());
			Serial.println(" Trying again in 5 seconds.");
			// Wait 5 second before retrying.
			delay(5000);
		}
	}
}

// The setup.
void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(230400);
	setup_wifi();
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
}

// The loop.
void loop() {
	// Confirm you're still connected to the MQtt server.
	if (!client.connected()) {
		reconnect();
	}
	client.loop();

	long now = millis();
	if (now - lastMsg > timeBetweenMessages) {
		lastMsg = now;
		++value;
		String payload = "{\"micros\":";
		payload += micros();
		payload += ",\"counter\":";
		payload += value;
		payload += ",\"client\":";
		payload += composeClientID();
		payload += "}";
		String pubTopic;
		pubTopic += topic;
		pubTopic += "/";
		pubTopic += composeClientID();
		pubTopic += "/out";
		Serial.print("Publish topic: ");
		Serial.println(pubTopic);
		Serial.print("Publish message: ");
		Serial.println(payload);
		client.publish((char*) pubTopic.c_str(), (char*) payload.c_str(), true);
	}
}