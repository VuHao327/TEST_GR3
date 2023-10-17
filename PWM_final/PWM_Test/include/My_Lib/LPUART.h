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

#ifndef MY_LIB_LPUART_H_
#define MY_LIB_LPUART_H_

/******************************************************************************
 *  DEFINES & MACROS
 *****************************************************************************/

#define RX 1u
#define TX 0u
#define E_IRQ 1u
#define N_IRQ 0u

/* LPUART Module Registers */
typedef struct {
	volatile uint32_t VERID;	// ID Register
	volatile uint32_t PARAM;	// Parameter Register
	volatile uint32_t GLOBAL;	// LPUART Global Register
	volatile uint32_t PINCFG;	// LPUART Pin Register
	volatile uint32_t BAUD;   // LPUART Baud Rate Register
	volatile uint32_t STAT;		// Status Register
	volatile uint32_t CTRL;		// Control Register
	volatile uint32_t DATA;		// Data Register
	volatile uint32_t MATCH;	// Match Address Register
	volatile uint32_t MODIR;	// Baud Rate Register
	volatile uint32_t FIFO;		// Baud Rate Register
	volatile uint32_t WATER;	// Baud Rate Register
} LPUART_Type;

/* LPUART0 base address */
#define LPUART0_BASE	(0x4006A000u)
/* LPUART0 base pointer */
#define LPUART0 		((LPUART_Type*)LPUART0_BASE)
/* LPUART1 base address */
#define LPUART1_BASE	(0x4006B000u)
/* LPUART1 base pointer */
#define LPUART1 		((LPUART_Type*)LPUART1_BASE)
/* LPUART2 base address */
#define LPUART2_BASE	(0x4006C000u)
/* LPUART2 base pointer */
#define LPUART2 		((LPUART_Type*)LPUART2_BASE)

/******************************************************************************
 *  EXTERN
 *****************************************************************************/


/******************************************************************************
 *  VARIABLES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

void LPUART_Init(void);
void LPUART1_send_char(char p_Data_c);
void LPUART1_send_string(char*  p_DataString_ptr);
void UART_Display(void);
void LPUART1_RxTx_IRQHandler(void);

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

#endif /* MY_LIB_LPUART_H_ */

/******************************************************************************
*                           End of File                                       *
******************************************************************************/
