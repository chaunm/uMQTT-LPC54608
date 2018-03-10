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

typedef struct MQTT_COMMUNICATOR_TAG* MQTT_COMMUNICATOR_HANDLE;

typedef void(*MQTT_COMM_ON_CONNECTED)(MQTT_COMMUNICATOR_HANDLE mqtt_comm, void* callbackCtx);
typedef void(*MQTT_COMM_ON_SUBCRIBE)(MQTT_COMMUNICATOR_HANDLE mqtt_comm, void* callbackCtx);
typedef void(*MQTT_COMM_ON_RECEIVED)(MQTT_COMMUNICATOR_HANDLE mqtt_com, MQTT_MESSAGE_HANDLE msgHandle, void* recvCbContext);
typedef void(*MQTT_COMM_ON_PUBLISH)(MQTT_COMMUNICATOR_HANDLE mqtt_comm, void* callbackCtx);

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
	XIO_HANDLE	xioHandle;
	uint16_t packetId;
	uint8_t state;
	MQTT_COMM_ON_CONNECTED fnConnectedCallback;
	MQTT_COMM_ON_SUBCRIBE fnSubCallback;
	MQTT_COMM_ON_RECEIVED fnReceivedCallback;
	MQTT_COMM_ON_PUBLISH fnPubCallback;
	void* connectedCbContext;
	void* subCbContext;
	void* recvCbContext;
	void* pubCbContext;
	union {
		TLSIO_CONFIG tlsConfig;
		SOCKETIO_CONFIG socketConfig;
	} xioConfigs;
} MQTT_COMMUNICATOR;

MQTT_COMMUNICATOR_HANDLE MQTT_Comm_Create(const char* host, int port, const char* clientId, const char* per,
		const char* userName, const char* password,
		bool security, const char* rootCa, const char* clientCrt, const char* privateKey, size_t rootCaSize, size_t clientCrtSize, size_t privKeySize);
void MQTT_Comm_Destroy(MQTT_COMMUNICATOR_HANDLE mqtt_comm);
void MQTT_Comm_Process(MQTT_COMMUNICATOR_HANDLE mqtt_comm);
int MQTT_Comm_Publish(MQTT_COMMUNICATOR_HANDLE mqtt_comm, const char* topic, const char* message, size_t size, uint8_t qos);
int MQTT_Comm_Subcribe(MQTT_COMMUNICATOR_HANDLE mqtt_comm, const char* topic, QOS_VALUE qos);
void MQTT_Comm_SetCallback(MQTT_COMMUNICATOR_HANDLE mqtt_comm, MQTT_COMM_ON_CONNECTED connectedCb, void* conCbContext, MQTT_COMM_ON_SUBCRIBE subCb, void* subCbContext,
		MQTT_COMM_ON_RECEIVED recvCb, void* recvCbContext, MQTT_COMM_ON_PUBLISH pubCb, void* pubCbContext);

#endif /* MQTT_MQTT_COMM_H_ */
