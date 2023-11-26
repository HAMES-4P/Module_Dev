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
	Init_Buzzer();
	_init_uart3();
    Init_DCMotors();

    // w_ref 288 -> PWM 95
	static const boolean get_log = true;
    static const float target_w_ref = 218;
	static const float PWM_speed = 72;
	static float speed_decel = 1.0;
    static unsigned char now_motor_value = 0;
    static int direction = 1;
    static int sw1_old = 0;
    static int sw1 = 0;
	static int distance = 0;
    static boolean mode = 0;

    while(1)
    {
		// pulling 방식을 사용한 SW1 작동
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

		/* laser module 수신 */
		distance = getTofDistance();
		if ( 100 < distance && distance <= 200) {
			speed_decel = 0.5;
			setBeepCycle(130);	// Beep every 0.5s
		} 
		else if ( 0 < distance && distance <= 100)
		{
			speed_decel = 0.0;
			setBeepCycle(130);	// Beep every 0.5s
		}
		else
		{
			speed_decel = 1.0;
			setBeepCycle(0);	// Beep off
		}

		// mode에 따라 PID 제어를 ON/OFF
        if ( !mode )
        {
            movChA_PWM(PWM_speed * speed_decel, direction);
            if ( get_log && !(getcntDelay() % 10000) )
            {
                my_printf("PWM: %.1f, Vw: %.1f\n", (PWM_speed * speed_decel), getWValue());
            }
        }
        else
        {
            now_motor_value = motor_pid(target_w_ref * speed_decel);
            movChA_PWM(now_motor_value, direction);
            if ( get_log && !(getcntDelay() % 10000) )
            {
                my_printf("Vw: %.2f\n", getWValue());
            }
        }
    }
    return (1);
}

