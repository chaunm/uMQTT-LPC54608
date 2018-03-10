/*
 * mqtt_app.h
 *
 *  Created on: Jan 13, 2018
 *      Author: A83571
 */

#ifndef MQTT_MQTT_APP_H_
#define MQTT_MQTT_APP_H_

extern sys_thread_t xMqttAppThread;
extern sys_thread_t xMqttMonitorThread;

void prvMqttAppTask(void* pvParameter);
void prvMqttMonitorTask(void* pvParameter);

#endif /* MQTT_MQTT_APP_H_ */
