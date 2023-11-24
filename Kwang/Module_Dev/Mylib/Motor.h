/*
 * Motor.h
 *
 *  Created on: 2023. 11. 24.
 *      Author: 현대오토9
 */

#ifndef MYLIB_MOTOR_H_
#define MYLIB_MOTOR_H_

void IsrGpt2T6Handler(void);

void Init_DCMotors(void);

void movChA(int dir);
void movChB(int dir);

void stopChA(void);
void stopChB(void);

void movChA_PWM(int duty, int dir);
void movChB_PWM(int duty, int dir);

#endif /* MYLIB_MOTOR_H_ */
