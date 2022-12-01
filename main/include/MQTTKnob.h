// Virtual knob/variable controlled via MQTT

#ifndef MQTTKNOB_H
#define MQTTKNOB_H

#include "Arduino.h"
#include "ArduinoMqttClient.h"
#include "string.h"

class MQTTKnob
{
private:
    /* data */
    MqttClient* client = nullptr;
    String topic = "";  // MQTT topic for this knob
    double state = 0;
public:
    MQTTKnob(MqttClient* client, String topic);
    ~MQTTKnob();

    double getState();
    void setState(double state);
    void publishState();
    bool processMessage(String topic, String message);
};


#endif
