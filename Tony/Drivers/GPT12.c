#include <gpt12.h>
#include <Ifx_reg.h>
#include <Ifx_Types.h>
#include <IfxGpt12.h>
#include <IfxPort.h>
#include <IO/Motor.h>
#include <math.h>
#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "IfxPort.h"
#include "Buzzer.h"
#include "asclin.h"
#include "isr_priority.h"

#define ISR_PROVIDER_GPT12_TIMER    0       //IfxSrc_Tos_cpu0
#define GPT1_BLOCK_PRESCALER        32      /* GPT1 block prescaler value                 */
#define TIMER_T3_INPUT_PRESCALER    32      /* Timer input prescaler value                */
#define GPT120_MODULE_FREQUENCY     100000000
#define PI                          3.14159265358979323

#define KP                          0.31
#define KI                          0.63
#define KD                          0.0003

#define ENC_A_CH    &MODULE_P02,4
#define ENC_B_CH    &MODULE_P02,5
#define PWM_A       &MODULE_P02,1
#define PWM_B       &MODULE_P10,3

static volatile unsigned int lMotorDuty = 20;
static volatile unsigned int rMotorDuty = 20;
static volatile unsigned int cnt_10us = 0;
static volatile unsigned int cntDelay = 0;
static volatile double theta, theta_old;
static volatile float w, w_old;

static void update_encoder(void);

IFX_INTERRUPT(IsrGpt2T6Handler, 0, ISR_PRIORITY_GPT2T6_TIMER);
void IsrGpt2T6Handler()
{
    if (cnt_10us  < lMotorDuty) {
        IfxPort_setPinHigh(PWM_A);  /*Left Motor (CH-A)*/
    } else {
        IfxPort_setPinLow(PWM_A);   /*Left Motor (CH-A)*/
    }

    /*
    if (cnt_10us < rMotorDuty) {
        IfxPort_setPinHigh(PWM_B);  // Right Motor (CH-B)
    } else {
        IfxPort_setPinLow(PWM_B);   // Right Motor (CH-B)
    }
    */
    if ( !(cnt_10us % 100) )
    {
        update_encoder();
        cnt_10us = 0;
    }

    cnt_10us++;
    cntDelay++;
}

float32 LPF(float32 y_old, float32 x, float32 Ts, float32 band)
{
    double A1=Ts/(Ts+1/band);
    float32 y = y_old+A1*(x-y_old);

    return y;
}

float getWValue()
{
    return w;
}

unsigned int getLeftMotorDuty()
{
	return lMotorDuty;
}

unsigned int getRightMotorDuty()
{
	return rMotorDuty;
}

void setLeftMotorDuty(unsigned char duty)
{
//	cnt_10us = 0;
	lMotorDuty = duty;
}

void setRightMotorDuty(unsigned int duty)
{
//	cnt_10us = 0;
	rMotorDuty = duty;
}

unsigned int getcntDelay()
{
	return cntDelay;
}

void setcntDelay(unsigned int n)
{
	cntDelay = n;
}

void init_gpt1(void)
{
    /* Initialize the GPT12 module */
    IfxGpt12_enableModule(&MODULE_GPT120); /* Enable the GPT12 module */

	/* Initialize the Timer T3 (PWM) */
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_32); /* Set GPT1 block prescaler: 32 */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);                       /* Set T3 to timer mode */
    IfxGpt12_T3_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);    /* Set T3 count direction(down) */
    IfxGpt12_T3_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_32); /* Set T3 input prescaler(2^5=32) */

	/* Calculate dutyUpTime and dutyDownTime for reloading timer T3 */
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, 100);       /* Set timer T3 value */

    /* Timer T2: reloads the value DutyDownTime in timer T3 */
    IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);                                  /* Set the timer T2 in reload mode */
    IfxGpt12_T2_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_bothEdgesTxOTL);    /* Reload Input Mode : Rising/Falling Edge T3OTL */
    IfxGpt12_T2_setTimerValue(&MODULE_GPT120, 100);

    /* Initialize the interrupt */
    volatile Ifx_SRC_SRCR *src = IfxGpt12_T3_getSrc(&MODULE_GPT120);                /* Get the interrupt address    */
    IfxSrc_init(src, ISR_PROVIDER_GPT12_TIMER, ISR_PRIORITY_GPT1T3_TIMER);          /* Initialize service request   */
    IfxSrc_enable(src);                                                             /* Enable GPT12 interrupt       */

    /* Initialize the Timer T4 for Ultrasonic */
    IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    IfxGpt12_T4_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_up);
    IfxGpt12_T4_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_32);
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, 0u);
}

void init_gpt2(void)
{
    /* Initialize the GPT12 module */
	IfxGpt12_enableModule(&MODULE_GPT120); /* Enable the GPT12 module */ /* Enable the GPT12 module */

    /* Initialize the Timer T6 for delay_ms */
    IfxGpt12_setGpt2BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_4);  /* Set GPT2 block prescaler: 4 */
    IfxGpt12_T6_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);                       /* Set T6 to timer mode */
    IfxGpt12_T6_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);    /* Set T6 count direction(down) */
    IfxGpt12_T6_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_1);  /* Set T6 input prescaler(2^0=1) */
    
    MODULE_GPT120.T6CON.B.T6OE = 0x1;                                               /* Overflow/Underflow Output Enable */   
    IfxGpt12_T6_setReloadMode(&MODULE_GPT120, IfxGpt12_TimerReloadMode_enable);     /* Reload from register CAPREL Enabled */
    IfxGpt12_T6_setTimerValue(&MODULE_GPT120, 250u);                                 /* Set T6 start value (10us) */
    MODULE_GPT120.CAPREL.U = 250u;                                                  /* Set CAPREL reload value */                                 /* Set CAPREL reload value */

    /* Initialize the interrupt */
    volatile Ifx_SRC_SRCR *src = IfxGpt12_T6_getSrc(&MODULE_GPT120);                /* Get the interrupt address    */
    IfxSrc_init(src, ISR_PROVIDER_GPT12_TIMER, ISR_PRIORITY_GPT2T6_TIMER);          /* Initialize service request   */
    IfxSrc_enable(src);                                                             /* Enable GPT12 interrupt       */
    
    IfxGpt12_T6_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void runGpt12_T3()
{
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void stopGpt12_T3()
{
	IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_stop);
}

void runGpt12_T6()
{
    IfxGpt12_T6_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void stopGpt12_T6()
{
    IfxGpt12_T6_run(&MODULE_GPT120, IfxGpt12_TimerRun_stop);
}

void runGpt12_T4()
{
    IfxGpt12_T4_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void stopGpt12_T4()
{
    IfxGpt12_T4_run(&MODULE_GPT120, IfxGpt12_TimerRun_stop);
}

void setGpt12_T4(unsigned short value)
{
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, value);
}

unsigned int getGpt12_T4()
{
	return IfxGpt12_T4_getTimerValue(&MODULE_GPT120);
}

void update_encoder(void)
{
    static const float Ts = 0.001;
    static boolean ENCA, ENCB;
    static int PosCnt = 0, PosCntd = 0;
    static int S, S_old=0;
    static int Direction, Pos_deg;
    static float32 Pos_rad;

    ENCA = IfxPort_getPinState(ENC_A_CH); // Encoder A signal state
    ENCB = IfxPort_getPinState(ENC_B_CH); // Encoder B signal state
    if (ENCA == FALSE && ENCB == FALSE)
    {
        S = 0;
    }
    else if(ENCA == FALSE && ENCB == TRUE)
    {
        S = 1;
    }
    else if(ENCA == TRUE && ENCB == TRUE)
    {
        S = 2;
    }
    else if(ENCA == TRUE && ENCB == FALSE)
    {
        S = 3;
    }

    // 모터 회전 방향 확인
    // S가 증가하면 정방향 direction = 1
    // S가 감소하면 역방향 direction = 0
    if (S - S_old==1 || S - S_old == -3)
    {
        PosCnt = PosCntd + 1;
        Direction = 1;
    }
    else if (S - S_old ==-1 || S - S_old == 3)
    {
        PosCnt = PosCntd - 1;
        Direction = 0;
    }
    S_old = S;


    Pos_rad = (PosCnt * PI) / 24;   //엔코더 라디안 값, 제어에 이용할 위치 정보
    Pos_deg = (int)(PosCnt*7.5);
    PosCntd = PosCnt;

    theta = Pos_rad;
    w = (float)(theta-theta_old)/Ts;
    w = LPF(w_old, w, Ts, 1);
    w_old = w;
    theta_old = theta;
}

unsigned char motor_pid(float w_ref)
{
    static const float32 Ts = 0.001;
    static float32 Vin;
    static float32 error_w = 0;
    static float32 error_w_int, error_w_int_old = 0;
    static float32 error_w_det, error_w_det_old = 0;
    static float32 error_w_det_filt = 0, error_w_det_filt_old = 0;

    error_w = w_ref - w;

    error_w_int=error_w_int_old + (error_w * Ts);
    error_w_int_old=error_w_int;

    error_w_det=(error_w_det_old-error_w)/Ts;
    error_w_det_old = error_w_det;

    error_w_det_filt = LPF(error_w_det_filt_old, error_w_det_filt, Ts, 1);
    error_w_det_filt_old = error_w_det_filt;

    if (error_w_int>10)
    {error_w_int=10;}

    Vin = (KP*error_w + KI*error_w_int + KD*error_w_det_filt);

    if (Vin>11.5)
    {
        Vin=11.5;
    }
    else if(Vin<0)
    {
        Vin=0;
    }

    Vin = ((Vin*100) / 12);
    return (unsigned char)Vin;
}
