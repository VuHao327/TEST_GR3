/*
 * LPIT.c
 *
 *  Created on: Sep 28, 2023
 *      Author: PC
 */
#include  "LPIT.h"
#include	"Gpio.h"
#include	"Lpuart.h"
#include <stdio.h>

unsigned int flag;



void LPIT0_Init(void)
{
  /*	Enable NVIC for LPIT chanel 0	*/
	/*	32 + 16 = 48 => ID NVIC interrupt LPIT- table excel attack Reference manual*/
	NVIC->ISER[1]	|= (1u<<16u);
	
	/*	clock sourse bit 24 -26 : option 7 = 011 ~ FIRCDIV2 , 110 ~ SPLLDIV2, 001 ~ SOSCDIV2	*/
	//PCC->PCC_LPIT |= (3u<<24u);	//FIRCDIV2 = 48Mhz
	// PCC->PCC_LPIT |= (6u<<24u);	//SPLLDIV2
	PCC->PCC_LPIT |= (1u<<24u);	//SOSCDIV2 = 1Mhz
	
	/*	enable clock sourse bit 30	*/
	PCC->PCC_LPIT |= (1u<<30u);
	
	/*	Enable chanel_0 timer interrupt */
	LPIT->LPIT_MIER |= (1u<<0u);
	
	/*	module clock enable	MCR[M_CEN] bit 0 	*/
	LPIT->LPIT_MCR |= (1u<<0u);
	
	/*	Run in debug mode MCR[DBG_EN] bit 3	*/
	LPIT->LPIT_MCR |= (1u<<3u);
	
		/*	Set value timer0 start	*/
	//LPIT->LPIT_TVAL0 = 0x2DC6C00;
	LPIT->LPIT_TVAL0 = 0xF4240;
	
	/*	Config chanel TCTRL[MODE] bit 2-3		*/
	LPIT->LPIT_TCTRL0 &= ~(3u<<2u);
	
	/* 	Timer chanel TCTRL[T_EN] bit 0	*/
	LPIT->LPIT_TCTRL0	|= (1u<<0u);
}

void LPIT1_Init(void)
{
  /*	Enable NVIC for LPIT chanel 1	*/
	/*	ID49 => ID NVIC interrupt LPIT- table excel attack Reference manual*/
	NVIC->ISER[1]	|= (1u<<(49u%32u));
		
	/*	Enable chanel_1 timer interrupt */
	LPIT->LPIT_MIER |= (1u<<1u);
	
	/*	Set value timer1 start	*/
	//LPIT->LPIT_TVAL1 = 0x2DC6C00;
	LPIT->LPIT_TVAL1 = 0xF4240;//1s
	
	/*	Config chanel TCTRL[MODE] bit 2-3		*/
	LPIT->LPIT_TCTRL1 &= ~(3u<<2u);
	
	/* 	Timer chanel TCTRL[T_EN] bit 0	*/
	LPIT->LPIT_TCTRL1	|= (1u<<0u);
}


void LPIT0_Ch0_IRQHandler (void)
{
	e_FlagSendUart = 1u;
  /*	Clear Flag interrupt Timer0	*/
	LPIT->LPIT_MSR = 0x1;
}

void LPIT0_Ch1_IRQHandler (void)
{

	e_CurrentTimeLP1_u32++;

  /*	Clear Flag interrupt Timer1	*/
	LPIT->LPIT_MSR = 0x2;
}

void LPIT_main(void)
{
  if (flag == 1)
  {
    Gpio_ToggleLed();
    flag = 0;
  }

}
