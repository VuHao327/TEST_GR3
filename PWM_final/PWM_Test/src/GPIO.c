	/******************************************************************************
|------------------------------------------------------------------------------
|   FILE DESCRIPTION
|------------------------------------------------------------------------------
|    File Name:   <GPIO>.<c>
|    Description:
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

uint8_t e_sw2_state_u8 = 0;
uint8_t e_sw3_state_u8 = 0;
uint8_t e_sw4_state_u8 = 0;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

/*
*******************************************************************************
Function name   :   Gpio_Init
Description     :   Using to:
					- Enable GPIO mode.
					- Setting direction.
					- Enable ISR if request.
*******************************************************************************
*/

void GPIO_Init(uint8_t p_Port_u8, uint8_t p_Pin_u8, uint8_t p_Direct_u8, uint8_t p_IRQ_u8){
	switch (p_Port_u8){
		case 'A':{
			/* Enable CLK */
			PCC->PCC_PORTA |= PCC_CGC_Mask;
			/* Set GPIO */
			PORTA->PCR[p_Pin_u8] |= (1u<<8);
			/* Enable IRQ */
			PORTA->PCR[p_Pin_u8] |= (p_IRQ_u8<<16);
			/* Set direction */
			GPIOA->PDDR |= (p_Direct_u8<<p_Pin_u8);
			break;
			}
		case 'B':{
			/* Enable CLK */
			PCC->PCC_PORTB |= PCC_CGC_Mask;
			/* Set GPIO */
			PORTB->PCR[p_Pin_u8] |= (1u<<8);
			/* Enable IRQ */
			PORTB->PCR[p_Pin_u8] |= (p_IRQ_u8<<16);
			/* Set direction */
			GPIOB->PDDR |= (p_Direct_u8<<p_Pin_u8);
			break;
			}
		case 'C':{
			/* Enable CLK */
			PCC->PCC_PORTC |= PCC_CGC_Mask;
			/* Set GPIO */
			PORTC->PCR[p_Pin_u8] |= (1u<<8);
			/* Enable IRQ */
			PORTC->PCR[p_Pin_u8] |= (p_IRQ_u8<<16);
			PORTC->PCR[p_Pin_u8] |= (2u);
			/* Set direction */
			GPIOC->PDDR |= (p_Direct_u8<<p_Pin_u8);
			break;
			}
		case 'D':{
			/* Enable CLK */
			PCC->PCC_PORTD |= PCC_CGC_Mask;
			/* Set GPIO */
			PORTD->PCR[p_Pin_u8] |= (1u<<8);
			/* Enable IRQ */
			PORTD->PCR[p_Pin_u8] |= (p_IRQ_u8<<16);
			/* Set direction */
			GPIOD->PDDR |= (p_Direct_u8<<p_Pin_u8);
			break;
			}
		case 'E':{
			/* Enable CLK */
			PCC->PCC_PORTE |= PCC_CGC_Mask;
			/* Set GPIO */
			PORTE->PCR[p_Pin_u8] |= (1u<<8);
			/* Enable IRQ */
			PORTE->PCR[p_Pin_u8] |= (p_IRQ_u8<<16);
			/* Set direction */
			GPIOE->PDDR |= (p_Direct_u8<<p_Pin_u8);
			break;
			}
		default:{
			/* Do nothing */
			break;
		}
	}
}

/*
*******************************************************************************
Function name   :   PORTC_IRQHandler
Description     :   Using to:
					- Set e_sw2_state_u8 = 1 if sw2 is pressed
					- Set e_sw3_state_u8 = 1 if sw3 is pressed
					- Set e_sw4_state_u8 = 1 if sw4 is pressed

*******************************************************************************
*/

void PORTC_IRQHandler(void){
  /* Check ISR flag */
	if(PORTC->PCR[12] & (1u<<24)){
		/* Clear IRQ flag */
		PORTC->PCR[12] |= (1u<<24);
		/* sw2 is pressed */
		e_sw2_state_u8 = 1;
	}
	/* Check ISR flag */
	else if(PORTC->PCR[13] & (1u<<24)){
		/* Clear IRQ flag */
		PORTC->PCR[13] |= (1u<<24);
		/* sw2 is pressed */
		e_sw3_state_u8 = 1;
	}
	/* Check ISR flag */
	else if(PORTC->PCR[3] & (1u<<24)){
		/* Clear IRQ flag */
		PORTC->PCR[3] |= (1u<<24);
		/* sw2 is pressed */
		e_sw4_state_u8 = 1;
		/* Toggle frequency change flag */
		e_freqState_u8 ^= 1;
	}
	else{
		/* Do nothing */
	}
}

/******************************************************************************
*                           End of File                                       *
******************************************************************************/
