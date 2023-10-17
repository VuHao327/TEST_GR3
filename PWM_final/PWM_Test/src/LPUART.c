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

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

/*
*******************************************************************************
Function name   :   f_LPUART_Config
Description     :   Using to config LPUART1.
*******************************************************************************
*/
void LPUART_Init(void)
{
  /*  1. Init Clock
	+ Enable FIRC clock
  */
	SCG->FIRCDIV |= (1U<<8); 				/* FIRCDIV2 divide by 1 */
  /*  2. Setting Tx/Rx pin
    + Enable clock for PORT : PCC[CGC] ~ bit30
    + Set alternate function pin : PORT_PCR[MUX] ~ bit 8-10 / Lpuart chanel 1 of PORTC, refer schematic
  */
  PCC->PCC_PORTC   |= (1u<<30u);           /* Enable clock for PORTC */
  PORTC->PCR[6]    |= (2u<<8u);            /*  lookup in file Excel to know what number is alt uart -Alternative 2 is lpuart_chanel1 */
  PORTC->PCR[7]    |= (2u<<8u);            /* Port C7: MUX = ALT2, UART1 TX */

 /* 3. Set source clock for Lpuart
    + peripheral clock source select: PCC[PCS] ~ bit 24-26
    + Enable clock for LPUART: PCC[CGC] ~ bit 30
  */

  PCC->PCC_LPUART1  |=  (3u<<24u);      /* Select source: FIRCDIV2_CLK = 48MHz */
  PCC->PCC_LPUART1  |=  (1u<<30u);      /* Enable clock for LPURAT */


 /* 4. Setting baudrate
    + Set the modulo divide rate: BAUD[SBR]
    + Set Oversampling ratio: BAUD[OSR]
  */
  LPUART1->BAUD &=(~( 0x1fu<<24u));               /* Set oversampling: 16*/
  LPUART1->BAUD &=~(0x1fffu);                     /* Clear */
  LPUART1->BAUD |= 312u;                          /* Set the modulo divide rate */

  /*  5. Setting frame
    + Select stop bit number: BAUD[SBNS]
    + Set data characters number: CTRL[M]
    +Set parity: CTRL[PE]
  */
  LPUART1->BAUD &=~ (1u<<13u);    /* one stop bit */
  LPUART1->CTRL &=~ (1u<<4u);     /* 8 bit data */
  LPUART1->CTRL &=~ (1u<<1u);     /* no parity */

/*  6. Enable Receiver Interrupt */
	LPUART1->CTRL |= (1u<<21u);

/*  7. Enable transmitter & receiver
  + Tranmitter Enable: CTRL[TE]
  + Receiver enable: CTRl[RE]
*/
  LPUART1->CTRL |= (1u<<18u);     /* Receiver Enable */
  LPUART1->CTRL |= (1u<<19u);     /* Transmitter Enable */
}

/*
*******************************************************************************
Function name   :   LPUART1_RxTx_IRQHandler
Description     :   Using to:
					- Change the frequency to 2KHz duty cycle 50% when receive character '2'
					- Change the frequency to 4KHz duty cycle 50% when receive character '4'
*******************************************************************************
*/

void LPUART1_RxTx_IRQHandler(void){
	uint8_t l_Data_u8 = 0;
	/* Check IRQ flag */
	if (LPUART1->STAT & (1u<<21)){
		l_Data_u8 = (uint8_t) LPUART1->DATA;
		if (l_Data_u8 == '2'){
			e_sw4_state_u8 = 1;
			e_freqState_u8 = 0;
//			FTM0->MOD = Freq_2MHz - 1;
//			e_Duty_2MHz_u32 = Freq_2MHz/2;
//			FTM0->C0V = e_Duty_2MHz_u32;
//			e_freqState_u8 = 0;
//			Gpio_SendToUart();
		}
		else if (l_Data_u8 == '4'){
			e_sw4_state_u8 = 1;
			e_freqState_u8 = 1;
//			FTM0->MOD = Freq_4MHz - 1;
//			e_Duty_4MHz_u32 = Freq_4MHz/2;
//			FTM0->C0V = e_Duty_4MHz_u32;
//			e_freqState_u8 = 1;
//			Gpio_SendToUart();
			}
		else {
			/* Do nothing */
		}
	}
	else{
		/* Do nothing */
	}
}

/*
*******************************************************************************
Function name   :   LPUART1_send_char
Description     :   Send single character
*******************************************************************************
*/

void LPUART1_send_char(char p_Data_c)
{
	/* Wait for transmit buffer to be empty */
	while((LPUART1->STAT & (1<<23u)) == 0);
	/* Send data */
	LPUART1->DATA = p_Data_c;
}

/*
*******************************************************************************
Function name   :   LPUART1_send_string
Description     :   Send string
*******************************************************************************
*/

void LPUART1_send_string(char*  p_DataString_ptr)
{
	/* Function to transmit whole string */
	while(*p_DataString_ptr != '\0') {
		LPUART1_send_char(*p_DataString_ptr);
		p_DataString_ptr++;
	}
}

/*
*******************************************************************************
Function name   :   UART_Display
Description     :   Using to display frequency and duty cycle
*******************************************************************************
*/

void UART_Display(void)
{
	if (e_freqState_u8 == 0)
	{
		char l_BufferVariable_s[3] = {0};
		char l_BufferFrequency_s[40] =  "Frequency = 2KHz, Duty Cycle = ";
		/* Convert duty cycle to string */
		sprintf(l_BufferVariable_s, "%ld\n\r", (e_Duty_2KHz_u32/Scale_2KHz));
		LPUART1_send_string(l_BufferFrequency_s);
		LPUART1_send_string(l_BufferVariable_s);

	}
	else{
		char l_BufferVariable_s[3] = {0};
		char l_BufferFrequency_s[40] =  "Frequency = 4KHz, Duty Cycle = ";
		/* Convert duty cycle to string */
		sprintf(l_BufferVariable_s, "%ld\n\r", (e_Duty_4KHz_u32/Scale_4KHz));
		LPUART1_send_string(l_BufferFrequency_s);
		LPUART1_send_string(l_BufferVariable_s);
	}
}

/******************************************************************************
*                           End of File                                       *
******************************************************************************/


