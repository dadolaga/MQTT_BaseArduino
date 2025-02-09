#include "Config.h"

#include "Mqtt.h"

const char* SSID = _SSID;
const char* PASSWORD = _PASSWORD;

const char* BROKER_IP = _MQTT_BROKER_IP;
const int BROKER_PORT = _MQTT_BROKER_PORT; //BROKER PORT

const char* USERNAME = _MQTT_USERNAME;
const char* USERPASSWORD = _MQTT_USER_PASSWORD;

Mqtt mqtt(SSID, PASSWORD, BROKER_IP, BROKER_PORT, USERNAME, USERPASSWORD);

void setup() {
  mqtt.init();

  // In this part subscribe to a mqtt topic
  // e.g.: mqtt.subscribe("test/test", function);

}

void loop() {
  mqtt.check();

  // ATTENTION: if you would receive a MQTT message run this command:
  //mqtt.check();

}