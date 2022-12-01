#include "MQTTKnob.h"
#include "string.h"
#include "DallasTemperature.h"
#include "MQTTValue.h"

// DS18B20 thermostat with MQTT Knob for changing set point

class Thermostat
{
private:
    String name = "";
    MQTTKnob *setPointKnob = nullptr;
    uint8_t outputPin;
    uint8_t sensorIndex = 0;
    DallasTemperature* DSReference;
    float hysteresis = 0.0;
    bool control = false;
    MQTTValue* currentTemp = nullptr;

    
public:
    void init();
    void stop();
    bool update();

    Thermostat(String name, MQTTKnob *spKnob, uint8_t outPin, DallasTemperature *DallasSensors, uint8_t DSSensorIndex, float hyst, MQTTValue* temp);
    ~Thermostat(){}
};