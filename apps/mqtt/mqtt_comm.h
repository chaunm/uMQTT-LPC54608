/*
 * mqtt_comm.h
 *
 *  Created on: Dec 30, 2017
 *      Author: A83571
 */

#ifndef MQTT_MQTT_COMM_H_
#define MQTT_MQTT_COMM_H_

#include "azure_umqtt_c/mqtt_client.h"
typedef struct MQTT_COMMUNICATOR_TAG
{
	MQTT_CLIENT_HANDLE 	mqttHandle;
	MQTT_CLIENT_OPTIONS mqttOptions;
	const char* host;
	const char* port;
	const char* per;
	bool security;
	const char* rootCa;
	const char* clientCert;
	const char* privateKey;
	size_t	rootCaSize;
	size_t 	clientCertSize;
	size_t 	privateKeySize;
	uint16_t packetId;
	bool connected;
} MQTT_COMMUNICATOR, *MQTT_COMMUNICATOR_HANDLE;

MQTT_COMMUNICATOR_HANDLE MQTT_Comm_Create();
#endif /* MQTT_MQTT_COMM_H_ */
