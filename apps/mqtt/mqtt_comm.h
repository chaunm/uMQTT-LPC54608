/*
 * mqtt_comm.h
 *
 *  Created on: Dec 30, 2017
 *      Author: A83571
 */

#ifndef MQTT_MQTT_COMM_H_
#define MQTT_MQTT_COMM_H_

#include "azure_umqtt_c/mqtt_client.h"
#include "azure_c_shared_utility/xio.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_c_shared_utility/tlsio_mbedtls.h"

enum MQTT_COMMUNICATOR_STATUS {
	MQTT_CONNECTED = 0,
	MQTT_DISCONNECTED,
	MQTT_CONNECTING,
	MQTT_ERROR
};

typedef void(*MQTT_COMM_ON_CONNECTED)(void* callbackCtx);
typedef void(*MQTT_COMM_ON_SUBCRIBE)(void* callbackCtx);
typedef void(*MQTT_COMM_ON_RECEIVED)(MQTT_MESSAGE_HANDLE msgHandle);
typedef void(*MQTT_COMM_ON_PUBLISH)(void* callbackCtx);

typedef struct MQTT_COMMUNICATOR_TAG
{
	MQTT_CLIENT_HANDLE 	mqttHandle;
	MQTT_CLIENT_OPTIONS mqttOptions;
	const char* host;
	int port;
	const char* per;
	bool security;
	TLSIO_CERT_INSTANCE rootCa;
	TLSIO_CERT_INSTANCE clientCert;
	TLSIO_KEY_INSTANCE privateKey;
	union {
		TLSIO_CONFIG tlsConfig;
		SOCKETIO_CONFIG socketConfig;
	} xioConfigs;
	XIO_HANDLE	xioHandle;
	uint16_t packetId;
	uint8_t state;
	MQTT_COMM_ON_CONNECTED fnConnectedCallback;
	MQTT_COMM_ON_SUBCRIBE fnSubCallback;
	MQTT_COMM_ON_RECEIVED fnReceivedCallback;
	MQTT_COMM_ON_PUBLISH fnPubCallback;
} MQTT_COMMUNICATOR, *MQTT_COMMUNICATOR_HANDLE;


#endif /* MQTT_MQTT_COMM_H_ */
