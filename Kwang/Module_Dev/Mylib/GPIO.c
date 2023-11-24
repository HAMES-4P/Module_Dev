/*
 * GPIO.c
 *
 *  Created on: 2023. 11. 24.
 *      Author: 현대오토9
 */
#include <IfxPort.h>
#include "GPIO.h"
#include "Bsp.h"

#define LED_BLUE     &MODULE_P10,2
#define LED_RED      &MODULE_P10,1

void Init_GPIO()
 {
    /* Set P10.2(LED1) as push-pull output */
    IfxPort_setPinModeOutput(LED_BLUE, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    /* Set P10.1(LED2) as push-pull output */
    IfxPort_setPinModeOutput(LED_RED, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
}

void blink_LED(void)
{
    IfxPort_togglePin(LED_BLUE);                                                     /* Toggle the state of the LED      */
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 200));    /* Wait 500 milliseconds            */
}

