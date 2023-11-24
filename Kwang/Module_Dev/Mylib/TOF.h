/*
 * TOF.h
 *
 *  Created on: 2023. 11. 24.
 *      Author: 현대오토9
 */

#ifndef MYLIB_TOF_H_
#define MYLIB_TOF_H_

static int verifyCheckSum(unsigned char data[]);
static int checkTofStrength(unsigned char data[]);

extern void Init_ToF(void);
extern int getTofDistance(void);

#endif /* MYLIB_TOF_H_ */
