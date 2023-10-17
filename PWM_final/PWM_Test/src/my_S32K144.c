	/******************************************************************************
|------------------------------------------------------------------------------
|   FILE DESCRIPTION
|------------------------------------------------------------------------------
|    File Name:   <my_S32K144>.<c>
|    Description:
|------------------------------------------------------------------------------
|
|------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|------------------------------------------------------------------------------
| Name: Group3
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

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

/*
*******************************************************************************
Function name   :   NVIC_Init
Description     :   Using to enable NVIC
*******************************************************************************
*/

void NVIC_Init(void){
	/* Enable LPUART1 NVIC */
	NVIC->ISER[1] |= (1u<<1);
	/* Enable PORTC NVIC */
	NVIC->ISER[1] |= (1u<<29);
}

/*
*******************************************************************************
Function name   :   SOSCDIV1_Init
Description     :   Using to config SOSC source clock
*******************************************************************************
*/

void SOSCDIV1_Init(uint8_t mode){
	SCG->SOSCCFG |= (1<<2);					/* EREFS = 1: Internal Crystal oscillator */
	SCG->SOSCCFG |= (1<<3); 				/* HGO   = 1: High-gain operator */
	SCG->SOSCCFG &= ~(3u<<4);				/* High Frequency range	*/
	SCG->SOSCCFG |= (3u<<4);
	SCG->SOSCDIV &= ~(7u);					/* Setting divided by mode */
	SCG->SOSCDIV |=  mode;
	SCG->SOSCCSR |= (1<<0);					/* Enable System OSC */
	while(!((SCG->SOSCCSR>>24) & 1));		/* Ensure system OSC is enable and output clock is valid */
}

/******************************************************************************
*                           End of File                                       *
******************************************************************************/
