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

#ifndef MY_LIB_FTM_H_
#define MY_LIB_FTM_H_

/******************************************************************************
 *  DEFINES & MACROS
 *****************************************************************************/

#define Freq_2KHz 2000u
#define Freq_4KHz 1000u
#define Scale_2KHz 200u
#define Scale_4KHz 100u

/* FlexTimer Registers */
typedef struct {
	volatile uint32_t SC ;
	volatile uint32_t CNT ;
	volatile uint32_t MOD ;
	volatile uint32_t C0SC ;
	volatile uint32_t C0V ;
	volatile uint32_t C1SC ;
	volatile uint32_t C1V ;
	volatile uint32_t C2SC ;
	volatile uint32_t C2V ;
	volatile uint32_t C3SC ;
	volatile uint32_t C3V ;
	volatile uint32_t C4SC ;
	volatile uint32_t C4V ;
	volatile uint32_t C5SC ;
	volatile uint32_t C5V ;
	volatile uint32_t C6SC ;
	volatile uint32_t C6V ;
	volatile uint32_t C7SC ;
	volatile uint32_t C7V ;
	volatile uint32_t CNTIN ;
	volatile uint32_t STATUS ;
	volatile uint32_t MODE ;
	volatile uint32_t SYNC ;
	volatile uint32_t OUTINIT ;
	volatile uint32_t OUTMASK ;
	volatile uint32_t COMBINE ;
	volatile uint32_t DEADTIME ;
	volatile uint32_t EXTTRIG ;
	volatile uint32_t POL ;
	volatile uint32_t FMS ;
	volatile uint32_t FILTER ;
	volatile uint32_t FLTCTRL ;
	volatile uint32_t QDCTRL ;
  	volatile uint32_t CONF ;
  	volatile uint32_t FLTPOL ;
  	volatile uint32_t SYNCONF ;
  	volatile uint32_t INVCTRL ;
  	volatile uint32_t SWOCTRL ;
  	volatile uint32_t PWMLOAD ;
  	volatile uint32_t HCR ;
  	volatile uint32_t PAIR0DEADTIME ;
  	uint32_t REVERSED0;
  	volatile uint32_t PAIR1DEADTIME ;
  	uint32_t REVERSED1;
  	volatile uint32_t PAIR2DEADTIME ;
  	uint32_t REVERSED2;
  	volatile uint32_t PAIR3DEADTIME ;
  	uint32_t REVERSED3[81];
  	volatile uint32_t MOD_MIRROR ;
} FTM_Type;

/* FTM0 base address */
#define FTM0_BASE	(0x40038000u)
/* FTM0 base pointer */
#define FTM0 		((FTM_Type *)FTM0_BASE)
/* FTM1 base address */
#define FTM1_BASE	(0x40039000u)
/* FTM1 base pointer */
#define FTM1 		((FTM_Type *)FTM1_BASE)
/* FTM2 base address */
#define FTM2_BASE	(0x4003A000u)
/* FTM2 base pointer */
#define FTM2 		((FTM_Type *)FTM2_BASE)

/******************************************************************************
 *  EXTERN
 *****************************************************************************/

extern uint32_t e_Duty_2KHz_u32;
extern uint32_t e_Duty_4KHz_u32;
extern uint8_t e_freqState_u8;

/******************************************************************************
 *  VARIABLES
 *****************************************************************************/

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

void FTM_PWM_Init(void);
void Frequency_Change(void);
void Duty_Cycle_Increase(void);
void Duty_Cycle_Decrease(void);

/******************************************************************************
 *  FUNCTION DECLARATION
 *****************************************************************************/

#endif /* MY_LIB_FTM_H_ */

/******************************************************************************
*                           End of File                                       *
******************************************************************************/
