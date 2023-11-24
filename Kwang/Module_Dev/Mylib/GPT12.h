/*
 * GPT12.h
 *
 *  Created on: 2023. 11. 24.
 *      Author: 현대오토9
 */

#ifndef MYLIB_GPT12_H_
#define MYLIB_GPT12_H_

int motor_pid(float w_ref);
unsigned int getLeftMotorDuty(void);
unsigned int getRightMotorDuty(void);
void setLeftMotorDuty(unsigned int duty);
void setRightMotorDuty(unsigned int duty);
unsigned int getcntDelay(void);
void setcntDelay(unsigned int n);
float getWValue(void);

void IsrGpt120T3Handler_Motor(void);
void IsrGpt2T6Handler(void);

void init_gpt1(void);
void init_gpt2(void);

void runGpt12_T3(void);
void stopGpt12_T3(void);
void runGpt12_T6(void);
void stopGpt12_T6(void);
void runGpt12_T4(void);
void stopGpt12_T4(void);
void setGpt12_T4(unsigned short value);
unsigned int getGpt12_T4(void);

void delay_ms(unsigned int delay_time);

#endif /* MYLIB_GPT12_H_ */
