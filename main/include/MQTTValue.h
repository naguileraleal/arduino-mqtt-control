#ifndef MQTTVALUE_H
#define MQTTVALUE_H

#include "Arduino.h"
#include "ArduinoMqttClient.h"
#include "string.h"

class MQTTValue
{
private:
    /* data */
    MqttClient* client = nullptr;
    int mode = INPUT;    // input/output
    String topic = "";  // MQTT topic for this value
    float state = 0;
public:
    MQTTValue(MqttClient* client, int mode, String topic);
    ~MQTTValue();

    float getState();
    void setState(float state);
    void publishState();
    bool processMessage(String topic, String message);
};


#endif
