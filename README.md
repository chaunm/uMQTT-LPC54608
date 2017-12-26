# uMQTT-LPC54608
## Overwiev
Porting the MQTT layer form Microsoft's Azure IoT hub SDK (https://github.com/Azure/azure-c-shared-utility) to use as common MQTT client for ARM Cortex-M MCU. 

More detail for porting please visit https://github.com/Azure/azure-c-shared-utility/tree/934bc731639f892bc26cbbae8b09274122505825/devdoc

## System description:
Board: NXP's OM13092

MCU: LPC54608 arm Cortex-M4

toolchain: NXP's MCUExpresso 10.1.0

OS: FreeRTOS

IP stack: LWIP

TLS: MbedTLS

The LPC54608 was built from NXP
