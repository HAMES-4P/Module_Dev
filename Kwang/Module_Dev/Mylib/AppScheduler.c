/*
 * AppScheduler.c
 *
 *  Created on: 2023. 11. 23.
 *      Author: 현대오토9
 */
#include "AppScheduler.h"
#include "Driver_Stm.h"
#include "ERU_Interrupt.h"
#include "GPIO.h"
#include "Terminal_stdio.h"
#include "Buzzer.h"
#include "Tof.h"

typedef struct
{
    uint32_t u32nuCnt1ms;
    uint32_t u32nuCnt10ms;
    uint32_t u32nuCnt100ms;
}TestCnt;

// Static Function Prototype
static void AppTask1ms();
static void AppTask10ms();
static void AppTask100ms();
static void AppTask500ms();

TestCnt stTestCnt;

void AppScheduling()
{
    if(stSchedulingInfo.u8nuScheduling100msFlag == 1u)
    {
        stSchedulingInfo.u8nuScheduling100msFlag = 0u;
        AppTask100ms();
    }
//    if(stSchedulingInfo.u8nuScheduling1msFlag == 1u)
//    {
//        stSchedulingInfo.u8nuScheduling1msFlag = 0u;
//        AppTask1ms(&status);
//
//        if(stSchedulingInfo.u8nuScheduling10msFlag == 1u)
//        {
//            stSchedulingInfo.u8nuScheduling10msFlag = 0u;
//            AppTask10ms(&status);
//        }
//
//        if(stSchedulingInfo.u8nuScheduling100msFlag == 1u)
//        {
//            stSchedulingInfo.u8nuScheduling100msFlag = 0u;
//            AppTask100ms();
//        }
//    }
    if(stSchedulingInfo.u8nuScheduling500msFlag == 1u)
    {
        stSchedulingInfo.u8nuScheduling500msFlag = 0u;
        AppTask500ms();
    }
    return;
}


void AppTask1ms()
{

}

void AppTask10ms()
{

}

void AppTask100ms()
{
    blink_LED();
}

void AppTask500ms()
{
    int distance = getTofDistance();

    my_printf("distance = %d\n", distance);

    if(distance > 0 && distance < 100)
    {
        setBeepCycle(200);
    }
    else
    {
        setBeepCycle(0);
    }
}

