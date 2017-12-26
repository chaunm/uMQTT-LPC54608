# uMQTT-LPC54608
## Overview
Porting the MQTT layer form Microsoft's Azure IoT hub SDK (https://github.com/Azure/azure-iot-sdk-c) to use as common MQTT client for ARM Cortex-M MCU. 

More detail for porting please visit https://github.com/Azure/azure-c-shared-utility/tree/934bc731639f892bc26cbbae8b09274122505825/devdoc

## System description:
Board: NXP's OM13092

MCU: LPC54608 arm Cortex-M4

toolchain: NXP's MCUExpresso 10.1.0

OS: FreeRTOS

IP stack: LWIP

TLS: MbedTLS

The LPC54608 SDK was built from NXP website https://mcuxpresso.nxp.com/en/welcome
## TLS support
This project create a mqtt client that support broker with x509 CA Cert and private key.

To understand how to create an MQTT broker with CA cert and private key please refer to this below link:

http://rockingdlabs.dunmire.org/exercises-experiments/ssl-client-certs-to-secure-mqtt
## Testing

Tested with mosquitto broker on Ubuntu with and without TLS support.

Playing ping pong with a device running on Cypress WICED platform.
