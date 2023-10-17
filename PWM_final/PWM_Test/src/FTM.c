	/******************************************************************************
|------------------------------------------------------------------------------
|   FILE DESCRIPTION
|------------------------------------------------------------------------------
|    File Name:   <exe>.<c>
|    Course:      EMB - Class 2
|    Module:      ASM - 2
|    Date:        dd-mm-yyy
|    Description: Source file for excercise <x>
|------------------------------------------------------------------------------
|
|------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|------------------------------------------------------------------------------
| Name: Hoang Phi Vu
|------------------------------------------------------------------------------
|               EXECUTION NOTE
|------------------------------------------------------------------------------
| Note: information for use when execute this program
| ---------------------------------------------------------------------------*/

/******************************************************************************
 *  INCLUDES
 *****************************************************************************/

#include "my_S32K144.h"

/******************************************************************************
 *  DEFINES & MACROS
 *****************************************************************************/

/******************************************************************************
 *  EXTERN
 *****************************************************************************/

/******************************************************************************
 *  VARIABLES
 *****************************************************************************/

uint32_t e_Duty_2KHz_u32 = 1000u;
uint32_t e_Duty_4KHz_u32 = 500u;
uint8_t e_freqState_u8 = 0u;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

/*
*******************************************************************************
Function name   :   FTM_PWM_Init
Description     :   Using to:
					- Enable FTM module.
					- Set Edge-Aligned PWM mode.
					- Set frequency as 2KHz or 4Khz.
					- Init duty cycle as 50%.
*******************************************************************************
*/

void FTM_PWM_Init(void)
{
	SOSCDIV1_Init(1);					/* SOSC = 8MHz/1 */
	PCC->PCC_PORTD |= (1u<<30);			/* Enable clock PORTD */
	PORTD->PCR[15] |= (1u<<9);			/* FTM0 channel0 */
	PCC->PCC_FTM0 |= (1u<<24); 			/* Select SOSC1 as CLK source */
	PCC->PCC_FTM0 |= (1u<<30); 			/* Enable CLK for FTM module */
	FTM0->MODE |= (1u<<2);				/* Disable write protection (WPDIS) */
	FTM0->SC = 0x00030001;				/* Enable PWM channel 0 output*/
										/* Enable PWM channel 1 output*/
										/* TOIE (Timer Overflow Interrupt Ena) = 0 (default) */
										/* CPWMS (Center aligned PWM Select) = 0 (default, up count) */
										/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) */
	FTM0->MOD = Freq_2KHz - 1;									/* PS (Prescaler factor) = 1. Prescaler = 2 */
	FTM0->COMBINE = 0x00000000;			/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0 */
	FTM0->POL = 0x00000000;				/* Polarity for all channels is active high (default) */
	FTM0->C0SC = 0x28;					/* FTM0 ch0: edge-aligned PWM, low true pulses */
										/* CHIE (Chan Interrupt Ena) = 0 (default) */
										/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM*/
										/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true */
	//FTM0->MOD = Freq_2KHz - 1;		/* FTM0 counter final value (used for PWM mode) */
										/* FTM0 Period = MOD-CNTIN+0x0001 ~= 4000000 ctr clks */
										/* 8MHz/2 = 4000KHz -> ticks -> 2KHz */
	FTM0->C0V = e_Duty_2KHz_u32;		/* FTM0 ch0 compare value (50% duty cycle) */
	FTM0->SC |= (3u<<3);				/* Start FTM0 counter with clk source = external clock (SOSCDIV1_CLK)*/
	e_freqState_u8 = 0; 				/* e_freqState_u8 = 0 when PWM frequency is 2KHz */
}

/*
*******************************************************************************
Function name   :   Frequency_Change
Description     :   Using to change frequency between 2KHz <-> 4KHz
*******************************************************************************
*/

void Frequency_Change(void){
	if (e_freqState_u8 == 0){
		FTM0->MOD = Freq_2KHz - 1;
		e_Duty_2KHz_u32 = Freq_2KHz/2;
		FTM0->C0V = e_Duty_2KHz_u32;
		e_freqState_u8 = 0;
	}
	else {
		FTM0->MOD = Freq_4KHz - 1;
		e_Duty_4KHz_u32 = Freq_4KHz/2;
		FTM0->C0V = e_Duty_4KHz_u32;
		e_freqState_u8 = 1;
	}
	e_sw4_state_u8 = 0;
}

/*
*******************************************************************************
Function name   :   Duty_Cycle_Increase
Description     :   Using to increase duty cycle by 10%
*******************************************************************************
*/



void Duty_Cycle_Increase(void){
	if (e_freqState_u8 == 0){
		/* if duty cycle < 100% */
		if (e_Duty_2KHz_u32 < Freq_2KHz){
			e_Duty_2KHz_u32 += Scale_2KHz; /* Increase duty cycle of 2KHz by 10% */
			FTM0->C0V = e_Duty_2KHz_u32;
		}
		else{
			// Do nothing
		}
	}
	else {
		/* if duty cycle < 100% */
		if (e_Duty_4KHz_u32 < Freq_4KHz){
			e_Duty_4KHz_u32 += Scale_4KHz; /* Increase duty cycle of 4KHz by 10% */
			FTM0->C0V = e_Duty_4KHz_u32;
		}
		else{
			/* Do nothing */
		}
	}
	e_sw2_state_u8 = 0; /* Clear increase flag */
}

/*
*******************************************************************************
Function name   :   Duty_Cycle_Decrease
Description     :   Using to decrease duty cycle by 10%
*******************************************************************************
*/

void Duty_Cycle_Decrease(void){
	if (e_freqState_u8 == 0){
		/* if duty cycle > 0% */
		if (e_Duty_2KHz_u32 > 0){
			e_Duty_2KHz_u32 -= Scale_2KHz; /* Decrease duty cycle of 2KHz by 10% */
			FTM0->C0V = e_Duty_2KHz_u32;

		}
		else{
			/* Do nothing */
		}
	}
	else {
		/* if duty cycle > 0% */
		if (e_Duty_4KHz_u32 > 0){
			e_Duty_4KHz_u32 -= Scale_4KHz; /* Decrease duty cycle of 4KHz by 10% */
			FTM0->C0V = e_Duty_4KHz_u32;

		}
		else{
			/* Do nothing */
		}
	}
	e_sw3_state_u8 = 0; /* Clear decrease flag */
}

/******************************************************************************
*                           End of File                                       *
******************************************************************************/


