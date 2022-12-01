#ifndef MQTTPIN_H
#define MQTTPIN_H

#include "Arduino.h"
#include "ArduinoMqttClient.h"
#include "string.h"

typedef enum {NONE, DIGITAL, ANALOG} PinType;

class MQTTPin
{
private:
    /* data */
    MqttClient* client = nullptr;
    pin_size_t pinId = 0;   // e.g: A0, D1, ...
    int mode = INPUT;    // input/output
    String topic = "";  // MQTT topic for this pin
    PinType type = NONE;
public:
    MQTTPin(MqttClient* client, byte pinId, int mode, String topic, PinType type);
    ~MQTTPin();

    int getState();
    void setState(int state);
    void publishState();
    bool processMessage(String topic, String message);
};


#endif
