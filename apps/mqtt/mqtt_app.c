/*
 * mqtt_app.c
 *
 *  Created on: Jan 13, 2018
 *      Author: A83571
 */

#include <stdlib.h>
#include "mqtt_comm.h"
#include "fsl_debug_console.h"
#include "certs/certs.h"

sys_thread_t xMqttAppThread = NULL;
sys_thread_t xMqttMonitorThread = NULL;

MQTT_COMMUNICATOR_HANDLE appMqttComm;

static const char* TOPIC_NAME_A = "LPC54608/TEST";
static const char* TOPIC_NAME_B = "LPC54608/TEST";
static const char* APP_NAME_A = "Message from LPC54608.";

/* MQTT CLIENT INFO */
static const char* clientId = "LPC54608";
#if MQTT_USE_TLS
static const char* host = "192.168.1.27";
static int port = 8883;
static const char* pers = "raspberrypi";
#else
static const char* host = "iot.eclipse.org";
static int port = 1883;
static const char* pers = "raspberrypi";
#endif //MQTT_USE_TLS
static void MqttAppConnectCallback(MQTT_COMMUNICATOR_HANDLE mqtt_comm, void* cbContext)
{
	PRINTF("ConnAck function called\r\n");
	if (MQTT_Comm_Subcribe(mqtt_comm, TOPIC_NAME_A, DELIVER_EXACTLY_ONCE) != 0)
	{
		PRINTF("%d: mqtt_client_subscribe failed\r\n", __LINE__);
	}
	PRINTF("mqtt_client_subscribe success\r\n");
}

static void MqttAppSubcribesCallback(MQTT_COMMUNICATOR_HANDLE mqtt_comm, void* cbContext)
{
	PRINTF("mqtt subscribe ack received\n");
	MQTT_Comm_Publish(mqtt_comm, TOPIC_NAME_A, APP_NAME_A, strlen(APP_NAME_A), DELIVER_EXACTLY_ONCE);
}

static void MqttAppRecvCallback(MQTT_COMMUNICATOR_HANDLE mqtt_comm, MQTT_MESSAGE_HANDLE msgHandle, void* cbContext)
{
	static uint32_t messageCount = 0;
	const APP_PAYLOAD* messagePayload = mqttmessage_getApplicationMsg(msgHandle);
	PRINTF("Incoming Msg: Packet Id: %d\r\nQOS: %d\r\nTopic Name: %s\r\nIs Retained: %s\r\nIs Duplicate: %s\r\nApp Msg: ",
			mqttmessage_getPacketId(msgHandle),
			mqttmessage_getQosType(msgHandle),
			mqttmessage_getTopicName(msgHandle),
			mqttmessage_getIsRetained(msgHandle) ? "true" : "false",
		    mqttmessage_getIsDuplicateMsg(msgHandle) ? "true" : "false"
	);
	for (size_t index = 0; index < messagePayload->length; index++)
	{
		PRINTF("%c", messagePayload->message[index]);
	}
	PRINTF("\r\nmessage count %d\r\n", ++messageCount);
#if MQTT_USE_TLS
	if (MQTT_Comm_Publish(mqtt_comm, TOPIC_NAME_B, APP_NAME_A, strlen(APP_NAME_A), DELIVER_EXACTLY_ONCE) != 0)
#else
	if (MQTT_Comm_Publish(mqtt_comm, TOPIC_NAME_A, APP_NAME_A, strlen(APP_NAME_A), DELIVER_EXACTLY_ONCE) != 0)
#endif
	{
		PRINTF("%d: mqtt_client_publish failed\r\n", __LINE__);
	}
}

static void MqttAppPubCallback(MQTT_COMMUNICATOR_HANDLE mqtt_comm, void* context)
{
	PRINTF("Message Publish finished\n");
}

void prvMqttAppTask(void* pvParameter)
{
//	static uint32_t taskNotifyValue;
//	if (xTaskNotifyWait(0x01, 0x01, &taskNotifyValue, portMAX_DELAY) == pdFALSE)
//	{
//		vTaskDelete(NULL);
//	}
#ifdef MQTT_USE_TLS
	appMqttComm = MQTT_Comm_Create(host, port, clientId, pers, NULL, NULL, true, rootCa, clientCert, privateKey, rootCaSize, clientCertSize, privateKeySize);
#else
#endif
	if (appMqttComm == NULL)
		vTaskDelete(NULL);
	MQTT_Comm_SetCallback(appMqttComm, MqttAppConnectCallback, NULL, MqttAppSubcribesCallback, NULL,
			MqttAppRecvCallback, NULL, MqttAppPubCallback, NULL);
	MQTT_Comm_Start_Connection(appMqttComm);
	while (1)
	{
		MQTT_Comm_Process(appMqttComm);
		if (xMqttMonitorThread != NULL)
			xTaskNotify(xMqttMonitorThread, 0x02, eNoAction);
		if (appMqttComm->state == MQTT_DISCONNECTED)
			break;
	}
	// while task still in process then the below code will not be entered
	if (xMqttMonitorThread != NULL)
		xTaskNotify(xMqttMonitorThread, 0x01, eNoAction);
	vTaskDelete(NULL);
}

void prvMqttMonitorTask(void* pvParameter)
{
	static uint32_t taskNotifyValue;
	if (xTaskNotifyWait(0x01, 0x01, &taskNotifyValue, portMAX_DELAY) == pdFALSE)
	{
		vTaskDelete(NULL);
	}
	xTaskCreate( prvMqttAppTask, "mqtt", 1024, NULL, 3, &xMqttAppThread );
	taskNotifyValue = 0;
	while (1)
	{
		if (xTaskNotifyWait(0x07, 0x07, &taskNotifyValue, pdMS_TO_TICKS(60000)) == pdTRUE)
		{
			if (taskNotifyValue &= 0x00000001)
			{
				PRINTF("MQTT task end, restart task\n");
				xTaskCreate( prvMqttAppTask, "mqtt", 1024, NULL, 3, &xMqttAppThread );
			}
			taskNotifyValue = 0;
		}
		else
		{
			PRINTF("MQTT task halted\n");
			MQTT_Comm_Destroy(appMqttComm);
			if (xMqttAppThread != NULL)
			{
				vTaskDelete(xMqttAppThread);
				xMqttAppThread = NULL;
			}
			xTaskCreate( prvMqttAppTask, "mqtt", 1024, NULL, 3, &xMqttAppThread );
		}
	}
	vTaskDelete(NULL);
}
