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
    Init_DCMotors();

    // w_ref 288 -> PWM 95
    static const float target_w_ref = 182;
    static unsigned char now_motor_value = 0;
    static int direction = 1;
    while(1)
    {
        now_motor_value = motor_pid(target_w_ref);
        movChA_PWM(now_motor_value, direction);
        if ( !(getcntDelay() % 10000) )
        {
            my_printf("%2d %.2f\n", now_motor_value, getWValue());
        }
    }
    return (1);
}

