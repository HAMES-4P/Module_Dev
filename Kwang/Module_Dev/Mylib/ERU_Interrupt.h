/*
 * ERU_Interrupt.h
 *
 *  Created on: 2023. 11. 23.
 *      Author: 현대오토9
 */

#ifndef MYLIB_ERU_INTERRUPT_H_
#define MYLIB_ERU_INTERRUPT_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxSrc.h"
#include "IfxScuEru.h"

/*********************************************************************************************************************/
/*-------------------------------------------------Data Structures---------------------------------------------------*/
/*********************************************************************************************************************/
typedef struct
{
    IfxScu_Req_In *reqPin;                      /* External request pin                                             */
    IfxScuEru_InputChannel inputChannel;        /* Input channel EICRm depending on input pin                       */
    IfxScuEru_InputNodePointer triggerSelect;   /* Input node pointer                                               */
    IfxScuEru_OutputChannel outputChannel;      /* Output channel                                                   */
    volatile Ifx_SRC_SRCR *src;                 /* Service request register                                         */
} ERUconfig;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
void initPeripheralsAndERU(void);

extern volatile unsigned int INTERRUT_VAL;

#endif /* MYLIB_ERU_INTERRUPT_H_ */
