/*
 * Lpuart.h
 *
 *  Created on: Sep 28, 2023
 *      Author: PC
 */

#ifndef LPUART_H_
#define LPUART_H_
#include "Register.h"

/** ===================Prototype===========================	*/
void Lpuart1_Init(void);
void Clock_Init(void);

unsigned char Uart_Transmit(unsigned char* u1_TxBuffer, unsigned int u4_Length);
unsigned char Uart_GetTransmitState(void);
void Uart_TxMainFunction(void);
unsigned int Uart_GetReceiveData ( unsigned char* u1_RxUserBuffer );
unsigned char Uart_GetReceiveState ( void );
void Uart_RxMainFunction(void);
void LPUART1_RxTx_IRQHandler(void);

void LPUART1_send_char(char data);
void LPUART1_send_string(char*  data_string );



#endif /* LPUART_H_ */
