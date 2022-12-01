#include "../include/Thermostat.h"
#include <DallasTemperature.h>
#include "Arduino.h"

Thermostat::Thermostat(String name, MQTTKnob *spKnob, uint8_t outPin, DallasTemperature *DallasSensors, uint8_t DSSensorIndex, float hyst, MQTTValue* temp) {

  this->name = name;
  setPointKnob = spKnob;
  outputPin = outPin;
  hysteresis = hyst;
  DSReference = DallasSensors;
  sensorIndex = DSSensorIndex;
  currentTemp = temp;
}

bool Thermostat::update() {
  bool result = true;
  // Request temp
  // TODO: DEBUGGING REMOVE!
  // DSReference->requestTemperaturesByIndex(sensorIndex);
  // float temp = DSReference->getTempCByIndex(sensorIndex);
  float temp = 0;
  currentTemp->setState(temp);
  // Check if reading was successful
  if (temp != DEVICE_DISCONNECTED_C) {
    if (control) {
      // Set output accordingly
      if (temp > setPointKnob->getState()) {
        digitalWrite(outputPin, LOW);
      } else if (temp < (setPointKnob->getState() - hysteresis)) {
        digitalWrite(outputPin, HIGH);
      }
    }
  } else {
    // return false indicating error
    result = false;
  }
  return result;
}

void Thermostat::init() {
  control = true;
}

void Thermostat::stop() {
  control = false;
}