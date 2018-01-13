/*
 * network.c
 *
 *  Created on: Dec 12, 2017
 *      Author: A83571
 */
#include <stdlib.h>
#include <lwipopts1.h>
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dns.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "app_gui.h"
#include "network.h"
#include "MQTTEcho.h"
#include "mqtt_app.h"

void stack_init(void *arg)
{
    static struct netif fsl_netif0;
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = EXAMPLE_PHY_ADDRESS,
        .clockName = EXAMPLE_CLOCK_NAME,
        .macAddress = configMAC_ADDR,
    };

    LWIP_UNUSED_ARG(arg);

    IP4_ADDR(&fsl_netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&fsl_netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&fsl_netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    tcpip_init(NULL, NULL);

    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw,
              &fsl_enet_config0, ethernetif0_init, tcpip_input);
    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" PING example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_ipaddr)[0], ((u8_t *)&fsl_netif0_ipaddr)[1],
           ((u8_t *)&fsl_netif0_ipaddr)[2], ((u8_t *)&fsl_netif0_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_netmask)[0], ((u8_t *)&fsl_netif0_netmask)[1],
           ((u8_t *)&fsl_netif0_netmask)[2], ((u8_t *)&fsl_netif0_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_gw)[0], ((u8_t *)&fsl_netif0_gw)[1],
           ((u8_t *)&fsl_netif0_gw)[2], ((u8_t *)&fsl_netif0_gw)[3]);
    PRINTF("************************************************\r\n");

    PRINTF("Setup DNS Server\n");
    ip_addr_t primaryDnsServer;
    // set up dns server.
    primaryDnsServer.type = IPADDR_TYPE_V4;
    ip4_addr_set_u32(&(primaryDnsServer.u_addr.ip4), ipaddr_addr("8.8.8.8"));
    dns_setserver(0, &primaryDnsServer);
    dns_init();
    PRINTF("Network stack setup finish\r\n");
//    vTaskDelay(pdMS_TO_TICKS(1000));
    if (xMqttAppThread != NULL)
    	xTaskNotify(xMqttAppThread, 0x01, eNoAction);
    vTaskDelete(NULL);
}
