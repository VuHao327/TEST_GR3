/******************************************************************************
|------------------------------------------------------------------------------
|   FILE DESCRIPTION
|------------------------------------------------------------------------------
|    File Name:   <GPIO>.<h>
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

#ifndef MY_LIB_GPIO_H_
#define MY_LIB_GPIO_H_

/******************************************************************************
 *  DEFINES & MACROS
 *****************************************************************************/

#define enable_IRQ 9u
#define disable_IRQ 0u
#define OUT 1
#define IN	0

/* GPIO Registers */
typedef struct {
    volatile uint32_t PDOR; // Port Data Output Register
    volatile uint32_t PSOR;	// Port Set Output Register
    volatile uint32_t PCOR;	// Port Clear Output Register
    volatile uint32_t PTOR;	// Port Toggle Output Register
    volatile uint32_t PDIR;	// Port Data Input Register
    volatile uint32_t PDDR;	// Port Data Direction Register
} GPIO_Type;

/* GPIOA base address */
#define GPIOA_BASE	(0x400FF000u)
/* GPIOA base pointer */
#define GPIOA ((GPIO_Type *)GPIOA_BASE)
/* GPIOB base address */
#define GPIOB_BASE	(0x400FF040u)
/* GPIOB base pointer */
#define GPIOB ((GPIO_Type *)GPIOB_BASE)
/* GPIOC base address */
#define GPIOC_BASE	(0x400FF080u)
/* GPIOC base pointer */
#define GPIOC ((GPIO_Type *)GPIOC_BASE)
/* GPIOD base address */
#define GPIOD_BASE	(0x400FF0C0u)
/* GPIOD base pointer */
#define GPIOD ((GPIO_Type *)GPIOD_BASE)
/* GPIOE base address */
#define GPIOE_BASE	(0x400FF100u)
/* GPIOE base pointer */
#define GPIOE ((GPIO_Type *)GPIOE_BASE)

/******************************************************************************
 *  EXTERN
 *****************************************************************************/

extern uint8_t e_sw2_state_u8;
extern uint8_t e_sw3_state_u8;
extern uint8_t e_sw4_state_u8;

/******************************************************************************
 *  VARIABLES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

void GPIO_Init(uint8_t p_Port_u8, uint8_t p_Pin_u8, uint8_t p_Direct_u8, uint8_t p_IRQ_u8);
void PORTC_IRQHandler(void);

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

#endif /* MY_LIB_GPIO_H_ */

/******************************************************************************
*                           End of File                                       *
******************************************************************************/
