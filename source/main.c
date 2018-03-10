/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54608.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "apps/GUI/app_gui.h"
#include "network.h"
#include "FreeRTOS.h"
#include "MQTTEcho.h"
#include "mqtt_app.h"
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */


int main(void) {
	CLOCK_EnableClock(kCLOCK_InputMux);
	BOARD_InitBootClocks();
  	/* Init board hardware. */
#if !USE_EXTERNAL_RAM
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitSDRAM();
#endif
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
    /* init GUI */
    BOARD_InitPWM();
#ifdef LCD_PRINT
    APP_GUI_Init();
#endif
    PRINTF("Start application\n");
    // Create lwip thread
    if(sys_thread_new("lwip init", stack_init, NULL, INIT_THREAD_STACKSIZE, 3) == NULL)
    	LWIP_ASSERT("lwip init(): Task creation failed.", 0);
    // Create mqtt thread
    xTaskCreate( prvMqttAppTask, "mqtt", 1024, NULL, 3, &xMqttAppThread );
    PRINTF("Creat mqtt process @ task ID %p\n", xMqttAppThread);
    xTaskCreate( prvMqttMonitorTask, "mqtt_monitor", 256, NULL, 3, &xMqttMonitorThread);
    /* RTOS task scheduler process */
    vTaskStartScheduler();

    return 0 ;
}
