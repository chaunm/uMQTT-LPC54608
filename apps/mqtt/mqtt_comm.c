/*
 * mqtt_comm.c
 *
 *  Created on: Dec 30, 2017
 *      Author: A83571
 */

#include <stdlib.h>
#include "mqtt_comm.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/shared_util_options.h"


static void Mqtt_Comm_OnRecvCallback(MQTT_MESSAGE_HANDLE  msgHandle, void* context)
{
	MQTT_COMMUNICATOR_HANDLE mqtt_comm = (MQTT_COMMUNICATOR_HANDLE)context;
	if (mqtt_comm->fnReceivedCallback != NULL)
		mqtt_comm->fnReceivedCallback(mqtt_comm, msgHandle, mqtt_comm->recvCbContext);
}

static void MQTT_Comm_OnOperationComplete(MQTT_CLIENT_HANDLE handle, MQTT_CLIENT_EVENT_RESULT actionResult, const void* msgInfo, void* callbackCtx)
{
//    (void)msgInfo;
//    (void)callbackCtx;
	MQTT_COMMUNICATOR_HANDLE mqtt_comm = (MQTT_COMMUNICATOR_HANDLE)callbackCtx;
    switch (actionResult)
    {
        case MQTT_CLIENT_ON_CONNACK:
        {
        	mqtt_comm->state = MQTT_CONNECTED;
        	if (mqtt_comm->fnConnectedCallback != NULL)
        		mqtt_comm->fnConnectedCallback(mqtt_comm, mqtt_comm->connectedCbContext);
            PRINTF("ConnAck function called\r\n");
            break;
        }
        case MQTT_CLIENT_ON_SUBSCRIBE_ACK:
        {
        	PRINTF("mqtt subscribe ack received\n");
        	if (mqtt_comm->fnSubCallback != NULL)
        		mqtt_comm->fnSubCallback(mqtt_comm, mqtt_comm->subCbContext);
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_ACK:
        {
//        	PRINTF("mqtt client publish ack received\n");
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_RECV:
        {
//        	PRINTF("MQTT_CLIENT_ON_PUBLISH_RECV\n");
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_REL:
        {
//        	PRINTF("MQTT_CLIENT_ON_PUBLISH_REL\n");
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_COMP:
        {
        	PRINTF("MQTT Client publish finished\n");
        	if (mqtt_comm->fnPubCallback != NULL)
        		mqtt_comm->fnPubCallback(mqtt_comm, mqtt_comm->pubCbContext);
            break;
        }
        case MQTT_CLIENT_ON_DISCONNECT:
        	mqtt_comm->state = MQTT_DISCONNECTED;
            break;
        case MQTT_CLIENT_ON_UNSUBSCRIBE_ACK:
        {
        	PRINTF("MQTT_CLIENT_ON_UNSUBSCRIBE_ACK\n");
            break;
        }
        case MQTT_CLIENT_ON_PING_RESPONSE:
        	break;
        default:
        {
            PRINTF("unexpected value received for enumeration (%d)\n", (int)actionResult);
        }
    }
}

static void MQTT_Comm_OnErrorComplete(MQTT_CLIENT_HANDLE mqtt_handle, MQTT_CLIENT_EVENT_ERROR error, void* callbackCtx)
{
	MQTT_COMMUNICATOR_HANDLE mqtt_comm = (MQTT_COMMUNICATOR_HANDLE)callbackCtx;
    switch (error)
    {
    case MQTT_CLIENT_CONNECTION_ERROR:
    case MQTT_CLIENT_PARSE_ERROR:
    case MQTT_CLIENT_MEMORY_ERROR:
    case MQTT_CLIENT_COMMUNICATION_ERROR:
    case MQTT_CLIENT_NO_PING_RESPONSE:
    case MQTT_CLIENT_UNKNOWN_ERROR:
        mqtt_comm->state = MQTT_ERROR;
        PRINTF("MQTT Error Code: %d\n", error);
        break;
    }
}

MQTT_COMMUNICATOR_HANDLE MQTT_Comm_Create(const char* host, int port, const char* clientId, const char* per,
		const char* userName, const char* password,
		bool security, const char* rootCa, const char* clientCrt, const char* privateKey, size_t rootCaSize, size_t clientCrtSize, size_t privKeySize)
{
	if ((host == NULL) || (port == 0))
		return NULL;
	if (security && ((rootCa == NULL) || (clientCrt == NULL) || (privateKey == NULL)))
		return NULL;
	MQTT_COMMUNICATOR_HANDLE mqtt_comm = (MQTT_COMMUNICATOR_HANDLE)malloc(sizeof(MQTT_COMMUNICATOR));
	if (mqtt_comm == NULL)
		return NULL;
	memset(mqtt_comm, 0, sizeof(MQTT_COMMUNICATOR));
	mqtt_comm->mqttHandle = mqtt_client_init(Mqtt_Comm_OnRecvCallback, MQTT_Comm_OnOperationComplete, (void*)(mqtt_comm), MQTT_Comm_OnErrorComplete, (void*)(mqtt_comm));
	if (mqtt_comm->mqttHandle == NULL)
	{
		free(mqtt_comm);
		return NULL;
	}
	mqtt_comm->security = security;
	mqtt_comm->host = host;
	mqtt_comm->port = port;
	mqtt_comm->mqttOptions.clientId = (char*)clientId;
	mqtt_comm->mqttOptions.willMessage = NULL;
	mqtt_comm->mqttOptions.username = (char*)userName;
	mqtt_comm->mqttOptions.password = (char*)password;
	mqtt_comm->mqttOptions.keepAliveInterval = 30; // keep alive interval should be >= 21 due to internal mqtt_client.c process
	mqtt_comm->mqttOptions.useCleanSession = true;
	mqtt_comm->mqttOptions.qualityOfServiceValue = DELIVER_EXACTLY_ONCE;
	if (security)
	{
		memset(&mqtt_comm->xioConfigs.tlsConfig, 0, sizeof(TLSIO_CONFIG));
		mqtt_comm->rootCa.certs = rootCa;
		mqtt_comm->rootCa.certsSize = rootCaSize;
		mqtt_comm->clientCert.certs = clientCrt;
		mqtt_comm->clientCert.certsSize = clientCrtSize;
		mqtt_comm->privateKey.key = privateKey;
		mqtt_comm->privateKey.keySize = privKeySize;
		mqtt_comm->xioConfigs.tlsConfig.hostname = mqtt_comm->host;
		mqtt_comm->xioConfigs.tlsConfig.port = mqtt_comm->port;
		mqtt_comm->xioHandle = xio_create(tlsio_mbedtls_get_interface_description(), (void *)&mqtt_comm->xioConfigs.tlsConfig);
		if (mqtt_comm->xioHandle == NULL)
		{
			mqtt_client_deinit(mqtt_comm->mqttHandle);
			free(mqtt_comm);
			return(NULL);
		}
		xio_setoption(mqtt_comm->xioHandle, OPTION_TRUSTED_CERT, (void*)&mqtt_comm->rootCa);
		xio_setoption(mqtt_comm->xioHandle, OPTION_X509_ECC_CERT, (void*)&mqtt_comm->clientCert);
		xio_setoption(mqtt_comm->xioHandle, OPTION_X509_ECC_KEY, (void*)&mqtt_comm->privateKey);
	}
	else
	{
		mqtt_comm->xioConfigs.socketConfig.hostname = mqtt_comm->host;
		mqtt_comm->xioConfigs.socketConfig.port = mqtt_comm->port;
		mqtt_comm->xioConfigs.socketConfig.accepted_socket = NULL;
		mqtt_comm->xioHandle = xio_create(socketio_get_interface_description(), &mqtt_comm->xioConfigs.socketConfig);
		if (mqtt_comm->xioHandle == NULL)
		{
			mqtt_client_deinit(mqtt_comm->mqttHandle);
		}
	}
	mqtt_comm->state = MQTT_DISCONNECTED;
	mqtt_comm->packetId = 0;
	return mqtt_comm;
}

void MQTT_Comm_Process(MQTT_COMMUNICATOR_HANDLE mqtt_comm)
{
	if (mqtt_comm == NULL)
		return;
	mqtt_comm->state = MQTT_CONNECTING;
	while (mqtt_client_connect(mqtt_comm->mqttHandle, mqtt_comm->xioHandle, &mqtt_comm->mqttOptions) != 0)
	{
		PRINTF("mqtt_client_connect failed\n");
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
	PRINTF("mqtt_client_connect success\n");
	mqtt_comm->state = MQTT_CONNECTED;
	while (1)
	{
		switch(mqtt_comm->state)
		{
		case MQTT_CONNECTED:
			mqtt_client_dowork(mqtt_comm->mqttHandle);
			break;
		case MQTT_ERROR:
			xio_close(mqtt_comm->xioHandle, NULL, NULL);
			xio_destroy(mqtt_comm->xioHandle);
			if (mqtt_comm->security == true)
			{
				mqtt_comm->xioHandle = xio_create(tlsio_mbedtls_get_interface_description(), (void *)&mqtt_comm->xioConfigs.tlsConfig);
				xio_setoption(mqtt_comm->xioHandle, OPTION_TRUSTED_CERT, (void*)&mqtt_comm->rootCa);
				xio_setoption(mqtt_comm->xioHandle, OPTION_X509_ECC_CERT, (void*)&mqtt_comm->clientCert);
				xio_setoption(mqtt_comm->xioHandle, OPTION_X509_ECC_KEY, (void*)&mqtt_comm->privateKey);
			}
			else
			{
				mqtt_comm->xioHandle = xio_create(socketio_get_interface_description(), &mqtt_comm->xioConfigs.socketConfig);
			}
			mqtt_comm->state = MQTT_CONNECTING;
			while (mqtt_client_connect(mqtt_comm->mqttHandle, mqtt_comm->xioHandle, &mqtt_comm->mqttOptions) != 0)
				vTaskDelay(pdMS_TO_TICKS(5000));
			mqtt_comm->state = MQTT_CONNECTED;
			break;
		case MQTT_DISCONNECTED:
			xio_close(mqtt_comm->xioHandle,  NULL, NULL);
			xio_destroy(mqtt_comm->xioHandle);
			mqtt_client_deinit(mqtt_comm->mqttHandle);
			free(mqtt_comm);
			goto END_PROCESS;
		default:
			break;
		}
	}
	END_PROCESS:
	return;
}

int MQTT_Comm_Publish(MQTT_COMMUNICATOR_HANDLE mqtt_comm, const char* topic, const char* message, size_t size, uint8_t qos)
{
	// check for input param
	if ((mqtt_comm == NULL) || (mqtt_comm->mqttHandle == NULL) || (mqtt_comm->state != MQTT_CONNECTED) ||
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
		mqtt_comm->state = MQTT_ERROR;
		return -1;
	}
	else
	{
		mqttmessage_destroy(msg);
		return 0;
	}
}

int MQTT_Comm_Subcribe(MQTT_COMMUNICATOR_HANDLE mqtt_comm, const char* topic, QOS_VALUE qos)
{
	SUBSCRIBE_PAYLOAD subscribe;
	subscribe.subscribeTopic = topic;
	subscribe.qosReturn = qos;
	if (mqtt_client_subscribe(mqtt_comm->mqttHandle, mqtt_comm->packetId++, &subscribe, sizeof(subscribe) / sizeof(SUBSCRIBE_PAYLOAD)) != 0)
	{
		PRINTF("%d: mqtt_client_subscribe failed\r\n", __LINE__);
		mqtt_comm->state = MQTT_ERROR;
		return -1;
	}
	return 0;
}

void MQTT_Comm_SetCallback(MQTT_COMMUNICATOR_HANDLE mqtt_comm, MQTT_COMM_ON_CONNECTED connectedCb, void* conCbContext, MQTT_COMM_ON_SUBCRIBE subCb, void* subCbContext,
		MQTT_COMM_ON_RECEIVED recvCb, void* recvCbContext, MQTT_COMM_ON_PUBLISH pubCb, void* pubCbContext)
{
	mqtt_comm->fnConnectedCallback = connectedCb;
	mqtt_comm->fnPubCallback = pubCb;
	mqtt_comm->fnReceivedCallback = recvCb;
	mqtt_comm->fnSubCallback = subCb;
	mqtt_comm->connectedCbContext = conCbContext;
	mqtt_comm->pubCbContext = pubCbContext;
	mqtt_comm->subCbContext = subCbContext;
	mqtt_comm->recvCbContext = recvCbContext;
}

