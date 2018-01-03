/*
 * mqtt_comm.c
 *
 *  Created on: Dec 30, 2017
 *      Author: A83571
 */

#include "mqtt_comm.h"

static int MQTT_Comm_Publish(MQTT_COMMUNICATOR_HANDLE mqtt_comm, const char* topic, const char* message, size_t size, uint8_t qos)
{
	// check for input param
	if ((mqtt_comm == NULL) || (mqtt_comm->mqttHandle == NULL) || (mqtt_comm->connected == false) ||
			(topic == NULL) || (message == NULL))
	{
		return -1;
	}
	MQTT_MESSAGE_HANDLE msg = mqttmessage_create(mqtt_comm->packetId, topic, qos, (const uint8_t*)message, size);
	if (msg == NULL)
		return -1;
	if (mqtt_client_publish(mqtt_comm->mqttHandle, msg) != 0)
	{
		mqttmessage_destroy(msg);
		return -1;
	}
	else
	{
		mqttmessage_destroy(msg);
		return 0;
	}
}
