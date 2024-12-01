#include "Mqtt.h"

const char* SSID = "SSID_WIFI";
const char* PASSWORD = "PASSWORD_WIFI";

const char* BROKER_IP = "BROKER_IP";
const int BROKER_PORT = 1883; //BROKER PORT

const char* USERNAME = "MQTT_USERNAME";
const char* USERPASSWORD = "MQTT_PASSWORD";

Mqtt mqtt(SSID, PASSWORD, BROKER_IP, BROKER_PORT, USERNAME, USERPASSWORD);

void setup() {
  mqtt.init();

  // In this part subscribe to a mqtt topic
  // e.g.: mqtt.subscribe("test/test", function);

}

void loop() {
  // ATTENTION: if you would receive a MQTT message run this command:
  //mqtt.check();

}