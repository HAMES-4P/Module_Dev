/*
 * Buzzer.h
 *
 *  Created on: 2023. 11. 24.
 *      Author: 현대오토9
 */

#ifndef MYLIB_BUZZER_H_
#define MYLIB_BUZZER_H_

void IsrGpt120T3Handler_Beep(void);
void Init_Buzzer(void);
void setBeepCycle(int cycle);
void Beep(unsigned int hz);
void delay_ms(unsigned int delay_time);

#endif /* MYLIB_BUZZER_H_ */
