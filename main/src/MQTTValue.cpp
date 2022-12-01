#include "../include/MQTTValue.h"

MQTTValue::MQTTValue(MqttClient *client, int mode, String topic)
{
    this->client = client;
    this->mode = mode;
    this->topic = topic;

    String aux = "controller/";
    aux.concat(topic);
    client->subscribe(aux);
}

MQTTValue::~MQTTValue()
{
}

float MQTTValue::getState()
{
    return state;
}

void MQTTValue::publishState() 
{
    char stateString[32];
    sprintf(stateString, "{\"State\":\"%f\"}", this->state);
    
    client->beginMessage(topic);
    client->println(stateString);
    client->endMessage();
}

void MQTTValue::setState(float state) 
{
    this->state = state;
}

// {SetState:1} sets pin state to 1
// {GetState} calls publishState method
bool MQTTValue::processMessage(String msgTopic, String message) 
{
  bool result = false;
  if (msgTopic.endsWith(topic))
  {
      if (message.indexOf("SetState") > 0)
      {
          float newState = message.substring(message.indexOf(":")+1, message.indexOf("}")).toFloat();
          setState(newState);
      } else if (message.indexOf("GetState") > 0)
      {
          publishState();
      }
      result = true;
  }
  return result;
}