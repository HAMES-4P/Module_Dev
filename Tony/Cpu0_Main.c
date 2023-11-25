#include "main.h"

IfxCpu_syncEvent g_cpuSyncEvent = 0;

int core0_main(void)
{
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    Init_Mystdio();
    Init_GPIO();
    setLED1(0);
    setLED2(0);
    Init_DCMotors();

    // w_ref 288 -> PWM 95
    static const float target_w_ref = 218;
    static unsigned char now_motor_value = 0;
    static int direction = 1;
    static int sw1_old = 0;
    static int sw1 = 0;
    static boolean mode = 0;

    while(1)
    {
        sw1 = getSW1();
        if (sw1_old != sw1)
        {
            if ( sw1_old && !sw1)
            {
                mode = !mode;
                setLED1(mode);
            }

            sw1_old = sw1;
        }


        if ( !mode )
        {
            movChA_PWM(72, direction);
            if ( !(getcntDelay() % 10000) )
            {
                my_printf("PWM: %2d\n", 72);
            }
        }
        else
        {
            now_motor_value = motor_pid(target_w_ref);
            movChA_PWM(now_motor_value, direction);
            if ( !(getcntDelay() % 10000) )
            {
                my_printf("w_ref: %.2f\n", getWValue());
            }
        }
    }
    return (1);
}

