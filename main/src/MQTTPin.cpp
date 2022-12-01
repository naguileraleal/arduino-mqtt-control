#include "../include/MQTTPin.h"

MQTTPin::MQTTPin(MqttClient *client, pin_size_t pinId, int mode, String topic, PinType type)
{
    this->client = client;
    this->pinId = pinId;
    this->mode = mode;
    this->topic = topic;
    this->type = type;

    String aux = "controller/";
    aux.concat(topic);
    client->subscribe(aux);

    // Initialize pin
    if (type == DIGITAL || type == ANALOG)
    {
        pinMode(pinId, mode);

        // Push notification to topic
        client->beginMessage(topic);
        client->print("{\"Mode\":\"");
        client->println(mode);
        client->print("\"}");
        client->endMessage();
    }
}

MQTTPin::~MQTTPin()
{
}

int MQTTPin::getState()
{
    int result = -1;
    if (type == ANALOG) 
    {
        result = analogRead(pinId);
    } else if (type == DIGITAL)
    {
        result = digitalRead(pinId);
    }
    return result;
}

void MQTTPin::publishState() 
{
    char stateString[16];
    client->beginMessage(topic);
    client->print("{\"State\":\"");
    client->print(itoa(getState(), stateString, 10));
    client->print("\"}");
    client->endMessage();
}

void MQTTPin::setState(int state) 
{
    if (mode == OUTPUT)
    {
        if (type == DIGITAL) {
            // Set pin state
            digitalWrite(pinId, state);
        } else if (type == ANALOG) {
            // Set pin state
            analogWrite(pinId, state);
        }
        // Send back new state
        publishState();
    }
}

// {SetState:1} sets pin state to 1
// {GetState} calls publishState method
bool MQTTPin::processMessage(String msgTopic, String message) 
{
  bool result = false;
  if (msgTopic.endsWith(topic))
  {
      if (message.indexOf("SetState") > 0)
      {
          int newState = message.substring(message.indexOf(":")+1, message.indexOf("}")).toInt();
          setState(newState);
      } else if (message.indexOf("GetState") > 0)
      {
          publishState();
      }
      result = true;
  }
  return result;
}