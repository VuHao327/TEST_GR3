/******************************************************************************
|------------------------------------------------------------------------------
|   FILE DESCRIPTION
|------------------------------------------------------------------------------
|    File Name:   <my_S32K144>.<h>
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

#ifndef MY_LIB_MY_S32K144_H_
#define MY_LIB_MY_S32K144_H_

/******************************************************************************
 *  INCLUDES
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "GPIO.h"
#include "FTM.h"
#include "LPUART.h"

/******************************************************************************
 *  DEFINES & MACROS
 *****************************************************************************/

/* SCG Configuration Registers  */
typedef struct {
  volatile const uint32_t VERID;	/* Version ID Register, offset */
  volatile const uint32_t PARAM;    /* Parameter Register, offset */
  uint32_t RESERVED0[2];
  volatile const uint32_t CSR;		/* Clock Status Register, offset */
  volatile uint32_t RCCR;			/* Run Clock Control Register */
  volatile uint32_t VCCR;           /* VLPR Clock Control Register */
  volatile uint32_t HCCR;           /* HSRUN Clock Control Register */
  volatile uint32_t CLKOUTCNFG;     /* SCG CLKOUT Configuration Register */
  uint32_t RESERVED1[55];
  volatile uint32_t SOSCCSR;        /* System OSC Control Status Register */
  volatile uint32_t SOSCDIV;        /* System OSC Divide Register */
  volatile uint32_t SOSCCFG;        /* System Oscillator Configuration Register */
  uint32_t RESERVED2[61];
  volatile uint32_t SIRCCSR;        /* Slow IRC Control Status Register */
  volatile uint32_t SIRCDIV;        /* Slow IRC Divide Register */
  volatile uint32_t SIRCCFG;        /* Slow IRC Configuration Register */
  uint32_t RESERVED3[61];
  volatile uint32_t FIRCCSR;        /* Fast IRC Control Status Register */
  volatile uint32_t FIRCDIV;        /* Fast IRC Divide Register, offset */
  volatile uint32_t FIRCCFG;        /* Fast IRC Configuration Register */
  uint32_t RESERVED4[189];
  volatile uint32_t SPLLCSR;        /* System PLL Control Status Register */
  volatile uint32_t SPLLDIV;        /* System PLL Divide Register, offset */
  volatile uint32_t SPLLCFG;        /* System PLL Configuration Register */
} SCG_Type;

/* SCG base address */
#define SCG_BASE	(0x40064000u)
/* SCG base pointer */
#define SCG			((SCG_Type *)SCG_BASE)

 /* NVIC Configuration Registers */
typedef struct {
	volatile uint32_t ISER[8];
	volatile uint32_t RESERVED0[24];
	volatile uint32_t ICER[8];
	volatile uint32_t RESERVED1[24];
	volatile uint32_t ISPR[8];
	volatile uint32_t RESERVED2[24];
	volatile uint32_t ICPR[8];
	volatile uint32_t res4[24];
	volatile uint32_t IABR[8];
	volatile uint32_t RESERVED3[56];
	volatile uint32_t IPR[60];
	volatile uint32_t RESERVED4[644];
	volatile uint32_t STIR;
} NVIC_Type;

/* NVIC base address */
#define NVIC_BASE	(0xE000E100u)
/* NVIC base pointer */
#define NVIC 		((NVIC_Type *)NVIC_BASE)

/* Clock Configuration Registers */
typedef struct {
	volatile uint32_t PCC_ADC1;			/* PCC ADC1 Register */
	volatile uint32_t RESERVED0[4];
	volatile uint32_t PCC_LPSPI0;		/* PCC LPSPI0 Register */
	volatile uint32_t PCC_LPSPI1;		/* PCC LPSPI1 Register */
	volatile uint32_t PCC_LPSPI2;		/* PCC LPSPI2 Register */
	volatile uint32_t RESERVED1[8];
	volatile uint32_t PCC_LPIT;			/* PCC LPIT Register */
	volatile uint32_t PCC_FTM0;			/* PCC FTM0 Register */
	volatile uint32_t PCC_FTM1;			/* PCC FTM1 Register */
	volatile uint32_t PCC_FTM2;			/* PCC FTM2 Register */
	volatile uint32_t PCC_ADC0;			/* PCC ADC0 Register */
	volatile uint32_t RESERVED3[13];
	volatile uint32_t PCC_PORTA;		/* PCC PORTA Register */
	volatile uint32_t PCC_PORTB;		/* PCC PORTB Register */
	volatile uint32_t PCC_PORTC;		/* PCC PORTC Register */
	volatile uint32_t PCC_PORTD;		/* PCC PORTD Register */
	volatile uint32_t PCC_PORTE;		/* PCC PORTE Register */
	volatile uint32_t RESERVED4[28];
	volatile uint32_t PCC_LPUART0;		/* PCC LPUART0 Register */
	volatile uint32_t PCC_LPUART1;		/* PCC LPUART1 Register */
	volatile uint32_t PCC_LPUART2;		/* PCC LPUART2 Register */
} PCC_Type;

/* PCC base address */
#define PCC_BASE		(0x4006509Cu)
/* PCC base pointer */
#define PCC				((PCC_Type *)PCC_BASE)
/* PCC_CGC enable bit mask */
#define  PCC_CGC_Mask	0x40000000u


/* Port Control Registers */
typedef struct {
    volatile uint32_t PCR[32];	/* Port Control Register */
} PORT_Type;

/* PCR_PORTA base address */
#define PCRA_BASE 	(0x40049000u)
/* PCR_PORTA base pointer */
#define PORTA 		((PORT_Type *)PCRA_BASE)
/* PCR_PORTB base address */
#define PCRB_BASE 	(0x4004A000u)
/* PCR_PORTB base pointer */
#define PORTB 		((PORT_Type *)PCRB_BASE)
/* PCR_PORTC base address */
#define PCRC_BASE 	(0x4004B000u)
/* PCR_PORTC base pointer */
#define PORTC 		((PORT_Type *)PCRC_BASE)
/* PCR_PORTD base address */
#define PCRD_BASE 	(0x4004C000u)
/* PCR_PORTD base pointer */
#define PORTD 		((PORT_Type *)PCRD_BASE)
/* PCR_PORTE base address */
#define PCRE_BASE 	(0x4004D000u)
/* PCR_PORTA base pointer */
#define PORTE 		((PORT_Type *)PCRE_BASE)

/* SysTick Timer Registers */
typedef struct {
	volatile uint32_t CTRL;		/* SysTick Control and Status Register */
	volatile uint32_t LOAD;		/* SysTick Reload Value Register */
	volatile uint32_t VAL;		/* SysTick Current Value Register */
	volatile uint32_t CALIB;	/* SysTick Calibration Register */
} SysTick_Type;

/* SysTick Timer base address */
#define SysTick_Timer_BASE	(0xE000E010u)
/* SysTick Timer base pointer */
#define SysTick_Timer ((SysTick_Type *)SysTick_Timer_BASE)

/******************************************************************************
 *  EXTERN
 *****************************************************************************/

/******************************************************************************
 *  VARIABLES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

void NVIC_Init(void);
void SOSCDIV1_Init(uint8_t mode);

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

#endif /* MY_LIB_MY_S32K144_H_ */

/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
