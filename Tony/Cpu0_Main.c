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

    unsigned char motor_value = 0;
    int direction = 1;
    while(1)
    {
        motor_value = motor_pid(20);
        movChA_PWM(motor_value, direction);
        if ( !(getcntDelay() % 100000) )
        {
            my_printf("%d %.2f\n", motor_value, getWValue());
        }
    }
    return (1);
}

