/*
 * network.h
 *
 *  Created on: Dec 12, 2017
 *      Author: A83571
 */

#ifndef NETWORK_NETWORK_H_
#define NETWORK_NETWORK_H_

#include "lwip/sys.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_ENET ENET

/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 1
#define configIP_ADDR3 102

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 1
#define configGW_ADDR3 1

/* MAC address configuration. */
#define configMAC_ADDR {0x02, 0x12, 0x13, 0x10, 0x15, 0x11}

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS 	(0x00U)

/* System clock name. */
#define EXAMPLE_CLOCK_NAME 		kCLOCK_CoreSysClk

/*! @brief Stack size of the temporary lwIP initialization thread. */
#define INIT_THREAD_STACKSIZE 	512

/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

void stack_init(void *arg);

#endif /* NETWORK_NETWORK_H_ */
