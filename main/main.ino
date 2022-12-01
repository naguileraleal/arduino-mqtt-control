#include "Arduino.h"
#include "include/MQTTClient.h"
#include "include/MQTTPin.h"
#include "include/WifiClient.h"
#include "timer.h"
#include "include/TempSensor.h"
#include "include/Thermostat.h"
#include "include/MQTTValue.h"

#define REPORT_PERIOD_MS 15000

// MQTT Pin topic definitions
#define LED_PIN_TOPIC "LED"
#define THERMOSTAT_SETPOINT_TOPIC "SETPOINT"
#define THERMOSTAT_TEMPERATURE_TOPIC "CURRENT_TEMPERATURE"
#define ERROR_TOPIC "ERR"

// Wifi and MQTT clients
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// MQTT pin declarations
MQTTPin* LEDPin = nullptr;
MQTTKnob* thermostatSetPointKnob = nullptr;
MQTTPin* MQTTPinArray[10] = {nullptr};
MQTTValue* tempValue = nullptr;

// Thermostat configuration
#define THERMOSTAT_NAME "Thermostat0"
#define THERMOSTAT_OUTPUT_PIN 13
#define THERMOSTAT_HYSTERESIS 1 // degrees celsius
#define THERMOSTAT_SENSOR_INDEX 0
Thermostat* thermostat = nullptr;

unsigned long prevMillis = 0;

// Timer to schedule pin state report
Timer sendTimer;

char MQTTMessage[32] = {'\0'};

void updatePins()
{
    Serial.println("Updating pin states");
    int i = 0;
    while (MQTTPinArray[i] != nullptr)
    {
        MQTTPinArray[i]->publishState();
        i++;
    }
    thermostatSetPointKnob->publishState();
    tempValue->publishState();
    Serial.println("Leaving updatePins()");
}

void onMessageCallback(int messageSize) 
{
    String topic = mqttClient.messageTopic();
    Serial.print("New message in ");
    Serial.println(topic);
    // Get the message
    if (messageSize > 32)
    {
        mqttClient.beginMessage(ERROR_TOPIC);
        mqttClient.println("MESSAGE TOO BIG");
        mqttClient.endMessage();
        return;
    }
    String message = mqttClient.readStringUntil('\n');
    Serial.print("Received message: ");
    Serial.println(message);
    int i = 0;
    while (MQTTPinArray[i] != nullptr)
    {
        MQTTPinArray[i]->processMessage(topic,message);
        i++;
    }
    thermostatSetPointKnob->processMessage(topic, message);
}

void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    initializeWifiClient();

    initializeMQTTClient(mqttClient);

    // sensors.begin();

    // Initialize MQTT pins
    LEDPin = new MQTTPin(
        &mqttClient,
        2,
        OUTPUT,
        LED_PIN_TOPIC,
        DIGITAL
    );

    // Initialize thermostat knob
    thermostatSetPointKnob = new MQTTKnob(
      &mqttClient,
      THERMOSTAT_SETPOINT_TOPIC
    );

    tempValue = new MQTTValue(
      &mqttClient,
      OUTPUT,
      THERMOSTAT_TEMPERATURE_TOPIC
    );

    thermostat = new Thermostat(
      THERMOSTAT_NAME,
      thermostatSetPointKnob,
      THERMOSTAT_OUTPUT_PIN,
      &sensors,
      THERMOSTAT_SENSOR_INDEX,
      THERMOSTAT_HYSTERESIS,
      tempValue
    );

    MQTTPinArray[0] = LEDPin;

    // Suscribe to controller topic
    mqttClient.onMessage(onMessageCallback);

    // Configure timers for repeated actions
    sendTimer.setInterval(REPORT_PERIOD_MS);
    sendTimer.setCallback(updatePins);
    sendTimer.start();
}

void loop()
{
  sendTimer.update();
  mqttClient.poll();
  thermostat->update();

  delay(150);
}