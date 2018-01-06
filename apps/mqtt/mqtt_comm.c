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

static void MQTT_Comm_OnOperationComplete(MQTT_COMMUNICATOR_HANDLE mqtt_comm, MQTT_CLIENT_EVENT_RESULT actionResult, const void* msgInfo, void* callbackCtx)
{
	MQTT_CLIENT_HANDLE handle = mqtt_comm->mqttHandle;
    (void)msgInfo;
    (void)callbackCtx;
    switch (actionResult)
    {
        case MQTT_CLIENT_ON_CONNACK:
        {
            PRINTF("ConnAck function called\r\n");
            SUBSCRIBE_PAYLOAD subscribe;
            subscribe.subscribeTopic = TOPIC_NAME_A;
            subscribe.qosReturn = DELIVER_EXACTLY_ONCE;
            if (mqtt_client_subscribe(handle, PACKET_ID_VALUE++, &subscribe, sizeof(subscribe) / sizeof(SUBSCRIBE_PAYLOAD)) != 0)
            {
                PRINTF("%d: mqtt_client_subscribe failed\r\n", __LINE__);
                g_continue = false;
            }
            PRINTF("mqtt_client_subscribe success\r\n");
            break;
        }
        case MQTT_CLIENT_ON_SUBSCRIBE_ACK:
        {
        	PRINTF("mqtt subscribe ack received\n");
            MQTT_MESSAGE_HANDLE msg = mqttmessage_create(PACKET_ID_VALUE++, TOPIC_NAME_A, DELIVER_EXACTLY_ONCE, (const uint8_t*)APP_NAME_A, strlen(APP_NAME_A));
            if (msg == NULL)
            {
                PRINTF("%d: mqttmessage_create failed\r\n", __LINE__);
                g_continue = false;
            }
            else
            {
                if (mqtt_client_publish(handle, msg))
                {
                    PRINTF("%d: mqtt_client_publish failed\r\n", __LINE__);
                    g_continue = false;
                }
                mqttmessage_destroy(msg);
            }
            // Now send a message that will get
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
            break;
        }
        case MQTT_CLIENT_ON_DISCONNECT:
            g_continue = false;
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

static void OnErrorComplete(MQTT_CLIENT_HANDLE handle, MQTT_CLIENT_EVENT_ERROR error, void* callbackCtx)
{
    switch (error)
    {
    case MQTT_CLIENT_CONNECTION_ERROR:
    case MQTT_CLIENT_PARSE_ERROR:
    case MQTT_CLIENT_MEMORY_ERROR:
    case MQTT_CLIENT_COMMUNICATION_ERROR:
    case MQTT_CLIENT_NO_PING_RESPONSE:
    case MQTT_CLIENT_UNKNOWN_ERROR:
        g_continue = false;
        PRINTF("MQTT Error Code: %d\n", error);
        break;
    }
}

MQTT_COMMUNICATOR_HANDLE MQTT_Comm_Create(const char* host, const char* port, const char* per,
		bool security, const char* rootCa, const char* clientCrt, const char* )
{
	MQTT_COMMUNICATOR_HANDLE mqtt_comm = (MQTT_COMMUNICATOR_HANDLE)malloc(sizeof(MQTT_COMMUNICATOR));
	if (mqtt_comm = NULL)
		return NULL;
}
