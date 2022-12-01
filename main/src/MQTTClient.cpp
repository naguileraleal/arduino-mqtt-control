#include "../include/MQTTClient.h"

void initializeMQTTClient(MqttClient& client)
{
    if (!client.connect(broker, port))
    {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(client.connectError());

        while (1)
            ;
    }

    Serial.println("You're connected to the MQTT broker!");
    Serial.println();
}
