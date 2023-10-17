#include "Register.h"
#include  "Gpio.h"
#include	"Lpuart.h"
#include	<stdio.h>

/*
 * Gpio.c
 *
 *  Created on: Sep 27, 2023
 *      Author: PC
 */

#define MAX_DURATION	( unsigned int)(10u)
#define MIN_DURATION	(unsigned int)(1u)

#define U1_OK  ((unsigned char)1u)
#define U1_NOTOK ((unsigned char)0u)  

unsigned int e_SettingHz_u32 = 0u;

unsigned int e_CurrentTimeLP1_u32 = 0u;

unsigned int CheckTimeValue = 0u;	
unsigned int DurationTime1_u32 = 10u;	
unsigned int g_LedState_u32 = 1u;

unsigned int FlagIncrease = 0u;
unsigned int Flag_Decrease = 0u;
unsigned int e_FlagSendUart = 0u;

unsigned int CheckValueButton2 = 0u;
unsigned char BufferVariable[100] = {0};

unsigned int Ex_CounterVariable __attribute__	((section(".my_variable"))) = 10;



void Gpio_Init_Button(void)
{
	/* Pins definitions
	 * ===================================================
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTC12             | GPIO [BUTTON1]
	 * PTC13             | GPIO [BUTTON2]
	 */
	/*	1.	Setting clocking	*/
	PCC->PCC_PORTC |= (1u << 30u); /*	CGC: bit 30 ,Enable clock for PORTC	*/

	/*	2.	Set pin as GPIO function	*/
	PORTC->PCR[BUTTON1] |= (1u << 8u); /*	MUX: bits 8-10 */
	PORTC->PCR[BUTTON2] |= (1u << 8u); /*	MUX: bits 8-10 */

	/*	3.	Disable Pull-up/pull-down	*/
	PORTC->PCR[BUTTON1] &= ~(1u << 1u); /*	PE: bit 1	*/
	PORTC->PCR[BUTTON2] &= ~(1u << 1u); /*	PE: bit 1	*/

	/*	4.	Set interrupt type */
	PORTC->PCR[BUTTON1] |= (9u << 16u); /*	IRQC: bit 16-19,Set interrupt rising edge */
	PORTC->PCR[BUTTON2] |= (10u << 16u); /*	IRQC: bit 16-19,Set interrupt falling edge */

	/*	5.	Set input Pin	*/
	GPIOC->PDDR &= ~(1u << BUTTON1); /*	Set Input for pin12 of PORTC	*/
	GPIOC->PDDR &= ~(1u << BUTTON2); /*	Set Input for pin13 of PORTC	*/

	/*	Enable NVIC for port C	*/
	NVIC->ISER[1] |= (1u << 29u);
	/*	32 + 29 = 61 => ID NVIC interrupt of PORTC- table excel attack Reference manual*/

}

void Gpio_Init_Led(void)
{
		/* Pins definitions
	 * ===================================================
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTD0              | GPIO [BLUE LED]
	 * PTD15             | GPIO [RED LED]
	 * PTD16             | GPIO [GREEN LED]
	 */

	/* 1.  Setting Clocking */
	PCC->PCC_PORTD |= (1u << 30u); /* CGC: bit 30, Enable clock for PORTD */

	/* 2. Set pins as GPIO function */
	PORTD->PCR[LED_BLUE]	|= (1 << 8u);
	PORTD->PCR[LED_RED]		|= (1 << 8u);
	PORTD->PCR[LED_GREEN]	|= (1 << 8u);

	/* 3. Set pins as output pin */
	GPIOD->PDDR |= (1 << 0u);
	GPIOD->PDDR |= (1 << 15u);
	GPIOD->PDDR |= (1 << 16u);
}

void Gpio_Init_PinClockOut(void)
{
		/* Pins definitions
	 * ===================================================
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTE10             | ClockOut
	 */
	/*	1.	Setting clocking	*/
	PCC->PCC_PORTE	|=	(1u	<<	30u);

	/*	2.	Set pin as ClockOut*/
	PORTE->PCR[PIN10]	|=	(2u	<<	8u);

}


void Gpio_SetPinValue(GPIO_Type *GPIO_Port, E_GPIO_Pin_Type GPIO_PinNum_E, const unsigned int GPIO_Value)
{
	if (GPIO_Value == 1u)
	{
		GPIO_Port->PDOR |= (1u << GPIO_PinNum_E);
	}
	else if (GPIO_Value == 0u)
	{
		GPIO_Port->PDOR &= ~(1u << GPIO_PinNum_E);
	}
}

void Gpio_GetPinValue(GPIO_Type *GPIO_Port, E_GPIO_Pin_Type GPIO_PinNum_E, unsigned int *GPIO_value)
{
	*GPIO_value = (GPIO_Port->PDIR >> GPIO_PinNum_E) & 1u;
}


unsigned int Port_ReadBitValue(PORT_Type *PORT_PCRn, E_GPIO_Pin_Type PORT_PinNum_E, unsigned int PORT_BitField)
{
	unsigned int Bit_Value = (PORT_PCRn->PCR[PORT_PinNum_E] >> PORT_BitField & 1u);

	return Bit_Value;
}


void Port_WriteBitValue(PORT_Type *PORT_PCRn, E_GPIO_Pin_Type PORT_PinNum_E, unsigned int PORT_BitField, unsigned int PORT_Value)
{
	if (PORT_Value == 1u)
	{
		PORT_PCRn->PCR[PORT_PinNum_E] |= (1u << PORT_BitField);
	}
	else
		PORT_PCRn->PCR[PORT_PinNum_E] &= ~(1u << PORT_BitField);
}

void Gpio_ClearAllLed(void)
{
	Gpio_SetPinValue(GPIOD, PIN0, LEVEL_HIGH);	/*  Turn off led_blue */
	Gpio_SetPinValue(GPIOD, PIN15, LEVEL_HIGH); /*  Turn off led_red  */
	Gpio_SetPinValue(GPIOD, PIN16, LEVEL_HIGH); /*  Turn off led_green  */
}


void Gpio_ToggleLed (void)
{
	GPIOD->PDOR ^= (1u<<0u);
}


void Gpio_ProcessPressButton(void)	__attribute__	((section(".my_flash_BlinkLed")));
void Gpio_ProcessPressButton(void)
{
	if (FlagIncrease)
	{
		DurationTime1_u32++; //increase 1 second

		Ex_CounterVariable++;

		/*	Check value increase duaration	*/
		if ( DurationTime1_u32 > MAX_DURATION)
		{
			DurationTime1_u32 = MAX_DURATION;	//set duration max = 10
			Ex_CounterVariable = MAX_DURATION;
			//send to uart error decrease
		}
		//reset value time
		CheckTimeValue = e_CurrentTimeLP1_u32;
		/*	reset flag interrupt increase	*/
		FlagIncrease = 0;
	}

	if (Flag_Decrease)
	{
		DurationTime1_u32--;//decrease 1 second
		Ex_CounterVariable--;
		/*	Check value decrease duaration	*/
		if((DurationTime1_u32) < MIN_DURATION )
		{
			DurationTime1_u32 = MIN_DURATION;	//Set duration min = 1
			Ex_CounterVariable = MIN_DURATION;
			// send to uart error increase
		}
		//reset value time
		CheckTimeValue = e_CurrentTimeLP1_u32;
		/*	reset flag interrupt decrease	*/
		Flag_Decrease = 0;
	}	
}

void Gpio_Blink_Led(void)
{
	/*	Read state button2	*/
	Gpio_GetPinValue(GPIOC,PIN13,&CheckValueButton2);
	
	/*	check time value	and button2 not press*/
	if (e_CurrentTimeLP1_u32 >= (CheckTimeValue + DurationTime1_u32)	&& CheckValueButton2 == 0)
	{
		g_LedState_u32++; 
		CheckTimeValue = e_CurrentTimeLP1_u32;
	}
	

	if (CheckValueButton2 == 1)
	{
		CheckTimeValue = e_CurrentTimeLP1_u32;
	}

	if (g_LedState_u32 >= 4u )
	{
		/*	reset cycle blink led	*/
		g_LedState_u32 = 1;
	}
	
	switch (g_LedState_u32)
	{
	case 1:
		Gpio_SetPinValue(GPIOD, PIN0, LEVEL_HIGH);	/*  Turn off led_blue */
		Gpio_SetPinValue(GPIOD, PIN15, LEVEL_LOW); /*  Turn On led_red  */
		Gpio_SetPinValue(GPIOD, PIN16, LEVEL_HIGH); /*  Turn off led_green  */
		break;
	case 2:
		Gpio_SetPinValue(GPIOD, PIN0, LEVEL_HIGH);	/*  Turn off led_blue */
		Gpio_SetPinValue(GPIOD, PIN15, LEVEL_HIGH); /*  Turn off led_red  */
		Gpio_SetPinValue(GPIOD, PIN16, LEVEL_LOW); /*  Turn On led_green  */
		break;
	case 3:
		Gpio_SetPinValue(GPIOD, PIN0, LEVEL_LOW);	/*  Turn off on_blue */
		Gpio_SetPinValue(GPIOD, PIN15, LEVEL_HIGH); /*  Turn off led_red  */
		Gpio_SetPinValue(GPIOD, PIN16, LEVEL_HIGH); /*  Turn off led_green  */
		break;
	}
}

void Gpio_TransmitUart(void)
{
	if (e_FlagSendUart == 1)
	{
		/*	Convert int to string for send uart	*/
		sprintf((char*)BufferVariable, "%d\n", Ex_CounterVariable);
		
		if( Uart_Transmit(BufferVariable,sizeof(BufferVariable)) == U1_OK)
		{
			e_FlagSendUart = 0;
		}
	
	}

}

void PORTC_IRQHandler(void)
{
		/* Check interrupt flag of BUTTON1 */
    if (Port_ReadBitValue(PORTC,PIN12,24u))
    {
			Port_WriteBitValue(PORTC, PIN12, 24u, LEVEL_HIGH);	/* Clear interrupt flag for BUTTON1 - Write 1 to clear */
			Flag_Decrease = 1;
			e_SettingHz_u32 = 1;       
    }
		/* Check interrupt flag of BUTTON2 */
    else if (Port_ReadBitValue(PORTC,PIN13,24u))
    {
			Port_WriteBitValue(PORTC, PIN13, 24u, LEVEL_HIGH); /* Clear interrupt flag for BUTTON2 - Write 1 to clear	*/
			FlagIncrease = 1;
			e_SettingHz_u32 = 2;			      
    }
}
