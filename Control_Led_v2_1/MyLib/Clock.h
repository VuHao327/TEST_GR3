/*
 * Clock.h
 *
 *  Created on: Sep 28, 2023
 *      Author: PC
 */

#ifndef CLOCK_H_
#define CLOCK_H_
#include  "Register.h"

#define FIRC_CLK	(1u)
#define SPLL_CLK	(2u)
#define SOSC_CLK  (3u)



void Clock_Init_SOSCDIV_CLk(void);
void Clock_Init_SPLL_CLK(void);
void Clock_Init_DivCore(void);
void Clock_ExportClockOut(unsigned int SourceClk);
void Clock_DisPlayToUart(void);
void Clock_Init_FIRCDIV(void);

#endif /* CLOCK_H_ */
