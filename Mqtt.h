#include "pins_arduino.h"
#include "api/Common.h"
#ifndef __MQTT__
#define __MQTT__

#include <Arduino.h>
#include <WiFiS3.h>
#include <ArduinoMqttClient.h>
#include <Arduino_JSON.h>
#include "Error.h"
#include "Map.h"

class Mqtt {
  private:
    WiFiClient* wifi;
    MqttClient* mqtt;
    LedError* error;
    MatrixError* matrix;
    Map<void (*)(JSONVar*)> map;

    const char* ssid;
    const char* password;
    const char* brokerIp;
    int brokerPort;
    const char* username;
    const char* userPassword;

    int ledPin;
    
    void connectToWifi();
    void connectToBroker();

    bool debug = false;
  public:
    Mqtt(const char* ssid, 
      const char* password, 
      const char* brokerIp, 
      int brokerPort, 
      const char* username,
      const char* userPassword,
      int ledPin = LED_BUILTIN);
    ~Mqtt();

    void init();
    void check(unsigned int everyMillis = 0);

    void send(const char* topic, char* text);
    void subscribe(const char* topic, void (*)(JSONVar*));
};

Mqtt::Mqtt(const char* ssid, 
      const char* password, 
      const char* brokerIp, 
      int brokerPort, 
      const char* username,
      const char* userPassword,
      int ledPin) {
  Mqtt::ssid = ssid;
  Mqtt::password = password;
  Mqtt::ledPin = ledPin;
  Mqtt::brokerIp = brokerIp;
  Mqtt::brokerPort = brokerPort;
  Mqtt::username = username;
  Mqtt::userPassword = userPassword;

  Mqtt::wifi = new WiFiClient();
  Mqtt::mqtt = new MqttClient(wifi);
  Mqtt::error = new LedError(ledPin);
  Mqtt::matrix = new MatrixError();

  #if DEBUG
  debug = true;
  #endif
}

Mqtt::~Mqtt() {
  delete wifi;
  delete mqtt;
  delete error;
}

void Mqtt::init() {
  Serial.begin(9600);

  error->init();

  connectToWifi();
  
  connectToBroker();

  matrix->complete();
}

void Mqtt::connectToWifi() {
  int count = 0;
  
  // Connet to wifi
  int status = WL_IDLE_STATUS;

  matrix->searchWifi();

  if (debug) {
    Serial.print("DEBUG: Try to connect to \"");
    Serial.print(ssid);
    Serial.println("\"");
  }

  while (status != WL_CONNECTED && count++ < 300) {
    status = WiFi.begin(ssid, password);

    if (debug) Serial.println("DEBUG: Conncting...");

    digitalWrite(ledPin, HIGH);
    delay(150);
    digitalWrite(ledPin, LOW);
  }

  if (status != WL_CONNECTED) {
    if (debug) Serial.println("DEBUG: Failed to connect to WiFi");

    error->wifiConnectionFail();
  }
}

void Mqtt::check(unsigned int everyMillis) {
  int messageSize = mqtt->parseMessage();

  if (messageSize) {    
    String topic = mqtt->messageTopic();
    String text = "";

    if (debug) {
      Serial.print("DEBUG: receved message from ");
      Serial.println(topic.c_str());
    }

    while (mqtt->available()) {
      text += (char) mqtt->read();
    }

    if (debug) {
      Serial.print("DEBUG: Text: ");
      Serial.println(text);
    }

    auto function = map.get(topic.c_str());

    if(function == nullptr) {
      if (debug) Serial.println("DEBUG: Function not found");
    } else {
      if (debug) Serial.println("DEBUG: Run Function");

      JSONVar* object = new JSONVar();
      *object = JSON.parse(text.c_str());

      (*function)(object);

      delete object;
    }
  }
}

void Mqtt::connectToBroker() {
  int count = 0;

  matrix->connectToServer();

  mqtt->setUsernamePassword(username, userPassword);

  if (debug) {
    Serial.print("DEBUG: ");
    Serial.print("try to connect to MQTT broker (");
    Serial.print(brokerIp);
    Serial.print(") with user: ");
    Serial.println(username);
  }

  while (!mqtt->connect(brokerIp, brokerPort) && count++ < 300) {
    if (debug) Serial.println("DEBUG: Try to connect...");

    digitalWrite(ledPin, HIGH);
    delay(150);
    digitalWrite(ledPin, LOW);
    delay(850);
  }

  switch (mqtt->connectError()) {
  case MQTT_CONNECTION_REFUSED:
    Serial.println("\tDetail: MQTT connection refused!"); error->serverNotResponse(); break;
  case MQTT_CONNECTION_TIMEOUT:
    Serial.println("\tDetail: MQTT connection timeout!"); error->serverNotResponse(); break;
  case MQTT_NOT_AUTHORIZED:
    Serial.println("\tDetail: Username or password wrong!"); error->serverNotResponse(); break;
  case MQTT_SERVER_UNAVAILABLE:
    Serial.println("\tDetail: MQTT server unavailable!"); error->serverNotResponse(); break;
  }
}

void Mqtt::send(const char *topic, char *text) {
  if (debug) {
    Serial.print("DEBUG: Send to ");
    Serial.print(topic);
    Serial.print(" message: \"");
    Serial.print(text);
    Serial.println("\"");
  }

  mqtt->beginMessage(topic);

  mqtt->print(text);

  mqtt->endMessage();
}

void Mqtt::subscribe(const char *topic, void (function)(JSONVar*)) {
  if (debug) {
    Serial.print("DEBUG: Subcribe to: ");
    Serial.println(topic);
  }

  mqtt->subscribe(topic);

  map.add(topic, function);
}

#endif