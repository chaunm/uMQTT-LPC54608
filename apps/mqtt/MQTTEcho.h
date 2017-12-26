#ifndef __MQTTECHO_H__
#define __MQTTECHO_H__

#include "sys_arch.h"
#define APP_QOS         DELIVER_EXACTLY_ONCE
extern sys_thread_t xMqttThread;
void prvMQTTEchoTask(void *pvParameters);
void vStartMQTTTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority);
#endif
