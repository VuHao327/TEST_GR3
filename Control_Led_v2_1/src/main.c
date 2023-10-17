/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "Gpio.h"
#include "LPIT.h"
#include "Clock.h"
#include	"Lpuart.h"



int main(void)
{
	Clock_Init_SPLL_CLK();	//6mhz
	Clock_Init_SOSCDIV_CLk();	//1mhz
	Clock_Init_FIRCDIV();
	Clock_Init_DivCore();

	LPIT0_Init();
	LPIT1_Init();

	Gpio_Init_Button();
	Gpio_Init_Led();
	Gpio_Init_PinClockOut();
	Gpio_ClearAllLed();

	Lpuart1_Init();
		
	while(1)
	{
		/*	Transmit Data from s32k to hercules	*/
		Uart_TxMainFunction();

		// void (*ptr)(void) = (void (*) (void)) 0x20006E01;
		// ptr();
		Gpio_TransmitUart();
		
		Gpio_ProcessPressButton();
		//__asm("bl Gpio_ProcessPressButton");
		
		/*	Gpio_Blink_Led	*/
		Gpio_Blink_Led();
	}
	return 0;
}
