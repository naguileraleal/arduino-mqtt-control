#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include "ArduinoMqttClient.h"
#include "WifiClient.h"

static const char broker[] = "test.mosquitto.org";
static int port = 1883;
static const char controllerTopic[] = "controller/*";

void initializeMQTTClient(MqttClient& client);

#endif