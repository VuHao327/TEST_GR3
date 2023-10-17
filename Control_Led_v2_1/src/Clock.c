/*
 * Clock.c
 *
 *  Created on: Sep 28, 2023
 *      Author: PC
 */
#include  "Clock.h"
#include	"Lpuart.h"
#include	"Gpio.h"

char str_SOSC[] = "===========Set Clock Out	SOSC: 1Mhz===========\n";
char str_SPLL[] = "===========Set Clock Out	SPLL: 6Mhz===========\n";


void Clock_Init_FIRCDIV(void)
{
	/*  enable clock FiRC DIV2  */
	SCG->FIRCDIV |= (1u << 8u);	//DIV2 by 1	

}

void Clock_Init_SOSCDIV_CLk(void)	/*	1Mhz*/
{
	/*	For any writeable SCG registers, only 32-bits writes are allowed */

	SCG->SOSCDIV=0x00000401; 	/* SOSCDIV1 = 1 & SOSCDIV2 = 8	*/
	SCG->SOSCCFG=0x00000024; 	/* Range=2: Medium freq (SOSC between 1MHz-8MHz)*/
														/* HGO=0: Config xtal osc for low power */
														/* EREFS=1: Input is external XTAL */
	while(SCG->SOSCCSR & (1u<<23u));	/*	check SOSCCSR unlock	*/
	SCG->SOSCCSR=0x00000001; 	/* LK=0: SOSCCSR can be written */
														/* SOSCCMRE=0: OSC CLK monitor IRQ if enabled */
														/* SOSCCM=0: OSC CLK monitor disabled */
														/* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */
														/* SOSCLPEN=0: Sys OSC disabled in VLP modes */
														/* SOSCSTEN=0: Sys OSC disabled in Stop modes */
														/* SOSCEN=1: Enable oscillator */
	while(!(SCG->SOSCCSR & (1<<24u))); /* Wait for sys OSC clk valid */
}


void Clock_Init_SPLL_CLK(void)	
{
  /*	For any writeable SCG registers, only 32-bits writes are allowed */

  /*	Step 0. check unlock	*/
	while (SCG->SPLLCSR &(1u<<23u)) {}
  
  /** Step 1.	disable SPLL*/
	SCG->SPLLCSR  = (0x00000000);
	
	/*	Step 2 and 3: Div1 by 1 and div2 by 8 	*/
	SCG->SPLLDIV = (0x00000401);	//div2 = 8

	/*	Step 4. Set PLL configuration	*/
	/*	
	*prediv = 8 
	*mul = 16 
	*source clock	= 1 ~ FastIRC	
	*/
	SCG->SPLLCFG = (0x00000701);
		
	/*check unlock	*/
	while (SCG->SPLLCSR &(1u<<23u)) {}
			
	/* Step 6. Enable Spll */
	SCG->SPLLCSR = (0x00000001);
		
	/*	Step 7. wait for SPLL to initialize */
	while (!(SCG->SPLLCSR & (1u<<24u))){}
	
}

void Clock_Init_DivCore(void)
{
	/*	mode = PLL clock source	-Divcore = 2 - div bus = 2 - divSlow = 3  */
	SCG->RCCR = (0x06010012);
	
	/* Wait for sync Clock = SPLL	*/
	while ((SCG->CSR & (0x0F000000))>>23u != 6){}
}


void Clock_ExportClockOut(unsigned int p_SourceClk)
{
	/*	Disable ClockOut: bit 11  = 0 */
	SIM->CHIPCTL	&= ~(1u<<11u);

	/* Wait clockOutEn == 0	*/
	while ((SIM->CHIPCTL & (0x800))>>11u == 1){}

		/*	choose clock sourse for display SCG clockout	*/
	if (p_SourceClk == FIRC_CLK)
	{
		SCG->CLKOUTCNFG = (0x03000000); // FIRC_CLK
	}
	else if (p_SourceClk == SPLL_CLK)
	{
		SCG->CLKOUTCNFG = (0x06000000); // SPLL_CLK
	}
	else if (p_SourceClk == SOSC_CLK)
	{
		SCG->CLKOUTCNFG = (0x1000000); // SOSC_CLK
	}
	

	/*	Clear 4 bit 4-7	*/
	SIM->CHIPCTL &= ~(0xF<<4u);

	/*	Config CLKOUTSEL bit 4-7 = */
	if (p_SourceClk == FIRC_CLK)
	{
		SIM->CHIPCTL	|= (6u<<4u);// FIRCDIV2
	}
	else if (p_SourceClk == SPLL_CLK)
	{
		SIM->CHIPCTL	|= (8u<<4u);	//SPLDIV2
	}
	else if (p_SourceClk == SOSC_CLK)
	{
		SIM->CHIPCTL	|= (2u<<4u);	//SOSCDIV2
	}
	
  /*	Config ClockDIV	bit 8-10 = div2 = 1 */

	SIM->CHIPCTL &= ~(5u<<8u); // div1

	/*	Enable SIM_CHIPSL	*/
	SIM->CHIPCTL	|= (1u<<11u);
}




void	Clock_DisPlayToUart(void)
{
	if (e_SettingHz_u32 == 1)
		{
			Gpio_SetPinValue(GPIOD, PIN0, LEVEL_LOW);	/*  Turn on led_blue */
			Gpio_SetPinValue(GPIOD, PIN15, LEVEL_HIGH); /*  Turn off led_red  */
			Gpio_SetPinValue(GPIOD, PIN16, LEVEL_HIGH); /*  Turn off led_green  */
			Clock_ExportClockOut(SOSC_CLK); // 1Mhz
			LPUART1_send_string	(str_SOSC);
			e_SettingHz_u32 = 0 ;
		}
		else if (e_SettingHz_u32 == 2)
		{
			Gpio_SetPinValue(GPIOD, PIN0, LEVEL_HIGH);	/*  Turn off led_blue */
			Gpio_SetPinValue(GPIOD, PIN15, LEVEL_LOW); /*  Turn off led_red  */
			Gpio_SetPinValue(GPIOD, PIN16, LEVEL_HIGH); /*  Turn off led_green  */
			Clock_ExportClockOut(SPLL_CLK);	//	6Mhz
			LPUART1_send_string	(str_SPLL);
			e_SettingHz_u32 = 0;
		}
		
}
