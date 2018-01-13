/*
 * mqtt_app.h
 *
 *  Created on: Jan 13, 2018
 *      Author: A83571
 */

#ifndef MQTT_MQTT_APP_H_
#define MQTT_MQTT_APP_H_

extern sys_thread_t xMqttAppThread;

void prvMqttAppTask(void* pvParameter);

#endif /* MQTT_MQTT_APP_H_ */
