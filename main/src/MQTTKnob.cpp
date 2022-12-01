#include <cstdlib>
#include "../include/MQTTKnob.h"

MQTTKnob::MQTTKnob(MqttClient *client, String topic)
{
    this->client = client;
    this->topic = topic;
    String aux = "controller/";
    aux.concat(topic);
    client->subscribe(aux);
}

MQTTKnob::~MQTTKnob()
{
}

double MQTTKnob::getState()
{
    return this->state;
}

void MQTTKnob::publishState() 
{
    char stateString[32];
    sprintf(stateString, "{\"State\":\"%f\"}", this->state);
    
    client->beginMessage(topic);
    client->println(stateString);
    client->endMessage();
}

void MQTTKnob::setState(double state) 
{
    this->state = state;
    // Send back new state
    publishState();
}

// {SetState:1} sets pin state to 1
// {GetState} calls publishState method
bool MQTTKnob::processMessage(String msgTopic, String message) 
{
  bool result = false;
  if (msgTopic.endsWith(topic))
  {
      if (message.indexOf("SetState") > 0)
      {
          char* lastChar;
          double newState = std::strtod(message.substring(message.indexOf(":")+1, message.indexOf("}")).c_str(), &lastChar);
          setState(newState);
      } else if (message.indexOf("GetState") > 0)
      {
          publishState();
      }
      result = true;
  }
  return result;
}