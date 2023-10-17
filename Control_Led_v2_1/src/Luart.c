/*
 * Luart.c
 *
 *  Created on: Sep 28, 2023
 *      Author: PC
 */
#include  "Lpuart.h"

/** =================Define===========================	*/
#define NULL_PTR  ((void*)0u)
#define U4_DATA_ZERO ((unsigned int)0u)
#define U1_OK  ((unsigned char)1u)
#define U1_NOTOK ((unsigned char)0u)     


#define U1_UART_TRANSMIT_IDLE      ((unsigned char)1u)
#define U1_UART_TRANSMIT_BUSY      ((unsigned char)2u)

#define U1_UART_RECEIVE_EMPTY     ((unsigned char)3u)
#define U1_UART_RECEIVE_NOTEMPTY  ((unsigned char)4u)

#define U4_UART_TRANSMIT_BUFFER_LENGTH_MAX    ((unsigned int)100ul)

#define U4_UART_RECEIVE_BUFFER_LENGTH_MAX    ((unsigned int)100ul)



/** =======================Type================================	*/

typedef struct Uart_TxBuff
{
    unsigned char u1_TxbufferData[U4_UART_TRANSMIT_BUFFER_LENGTH_MAX];
    unsigned int u4_TxBuffLength;
} Uart_TxBuff_Str;

typedef struct Uart_RxBuff
{
    unsigned char u1_RxbufferData[U4_UART_TRANSMIT_BUFFER_LENGTH_MAX];
    unsigned int u4_WriteCounter;
    unsigned int u4_ReadCounter;
} Uart_RxBuff_Str;

/** =================Variable===========================	*/

static Uart_TxBuff_Str Uart_TxBuff;
static volatile Uart_RxBuff_Str Uart_RxBuff;

/* Transmit State: IDLE, BUSY */
static unsigned char u1_TransmitState;
static volatile unsigned char u1_ReceiveState;

/* Buffer for function Uart_TxMainFunction */
static unsigned int u4_BufferCount_TxMain = U4_DATA_ZERO;

/*  variable for function GetReceiveData  */
static unsigned int u4_ReceiveLength_Get = U4_DATA_ZERO ;
static unsigned int u4_RxCountLength_Get = U4_DATA_ZERO;

void Lpuart1_Init(void)
{
  /* 1.  Setting NVIC */
	//NVIC->ISER[1] = (1u << (33 % 32));
	
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
  LPUART1->BAUD &=(~( 0x1fu<<24u));    /* Set oversampling: 16*/
  LPUART1->BAUD &=~(0x1fffu);                     /* Clear */
	LPUART1->BAUD |= 312u;                          /* Set the modulo divide rate */

  /*  5. Setting frame 
    + Select stop bit number: BAUD[SBNS]
    + Set data characters number: CTRL[M]
    +Set parity: CTRL[PE]
  */
  LPUART1->BAUD &=~ (1u<<13u);    /* ont stop bit */
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

  /*  Init buffer */
  Uart_TxBuff.u4_TxBuffLength = U4_DATA_ZERO;

  Uart_RxBuff.u4_ReadCounter = U4_DATA_ZERO;
  Uart_RxBuff.u4_WriteCounter = U4_DATA_ZERO;

  u1_TransmitState = U1_UART_TRANSMIT_IDLE;
  u1_ReceiveState = U1_UART_RECEIVE_EMPTY;

}

unsigned int u4_LenCount = U4_DATA_ZERO;
unsigned char Uart_Transmit(unsigned char* u1_TxBuffer, unsigned int u4_Length)
{
    unsigned char u1_TransmitResult;
    if (( u1_TransmitState == U1_UART_TRANSMIT_IDLE )
        && ( u4_Length <= U4_UART_TRANSMIT_BUFFER_LENGTH_MAX )
        && ( u1_TxBuffer != NULL_PTR ))
    {
      if (u4_LenCount < u4_Length)
      {
        Uart_TxBuff.u1_TxbufferData[u4_LenCount] = *(u1_TxBuffer + u4_LenCount);
        u4_LenCount++;
      }
      else if(u4_LenCount == u4_Length)
      {
      	u4_LenCount = U4_DATA_ZERO;
        Uart_TxBuff.u4_TxBuffLength = u4_Length;
        u1_TransmitResult = U1_OK;
        u1_TransmitState = U1_UART_TRANSMIT_BUSY;
      }     
    }
    else
    {
        u1_TransmitResult = U1_NOTOK;
    }
    return (u1_TransmitResult);
}

unsigned char Uart_GetTransmitState(void)
{
  return (u1_TransmitState); /* Return: 0/IDLE/BUSY */
}


void Uart_TxMainFunction(void)
{
    if (u1_TransmitState == U1_UART_TRANSMIT_BUSY )
    {
        /* check for transmit buffer to be empty */
				if((LPUART1->STAT & (1<<23u)))
        {
						if ( Uart_TxBuff.u4_TxBuffLength != U4_DATA_ZERO )
            {
                /* Send data to Hercules */
                LPUART1->DATA = Uart_TxBuff.u1_TxbufferData[u4_BufferCount_TxMain];
                u4_BufferCount_TxMain++; 

                if (u4_BufferCount_TxMain == Uart_TxBuff.u4_TxBuffLength )
                {
                  u4_BufferCount_TxMain = U4_DATA_ZERO;
									u1_TransmitState = U1_UART_TRANSMIT_IDLE;
                }
            }
        }      
    }
}

unsigned int Uart_GetReceiveData ( unsigned char* u1_RxUserBuffer )	
{
		/*  check Is there data? Bit 20 of Reg STAT, Bit20 True when Uart receives the full string from hecules */
     if (LPUART1->STAT & (1<<20u))
     {
        /*  If Ptr Read != Ptr Write => Have Data in Buffer IRQ uart  */
       if (Uart_RxBuff.u4_ReadCounter != Uart_RxBuff.u4_WriteCounter)	
       {
         u1_RxUserBuffer[u4_RxCountLength_Get] = Uart_RxBuff.u1_RxbufferData[Uart_RxBuff.u4_ReadCounter];
         u4_RxCountLength_Get++;
         Uart_RxBuff.u4_ReadCounter++;

         if (Uart_RxBuff.u4_ReadCounter >= U4_UART_RECEIVE_BUFFER_LENGTH_MAX)
         {
           Uart_RxBuff.u4_ReadCounter = 0;
         }
       }
       else
       {
         u4_ReceiveLength_Get	=	u4_RxCountLength_Get;
         u4_RxCountLength_Get = 0;
         u1_ReceiveState = U1_UART_RECEIVE_EMPTY;
         /* Reset bit 20 : IDLE */
         LPUART1->STAT = 0x100000;
         //Ex_Process_data = 1;
       }
		 }
    return (u4_ReceiveLength_Get);
}


unsigned char Uart_GetReceiveState(void)
{
    return (u1_ReceiveState);
}

void Uart_RxMainFunction ( void )
{
    /* Read data from hardware buffer */
    Uart_RxBuff.u1_RxbufferData[Uart_RxBuff.u4_WriteCounter] = (unsigned char)LPUART1->DATA;
		Uart_RxBuff.u4_WriteCounter++;
    if (  Uart_RxBuff.u4_WriteCounter >= U4_UART_RECEIVE_BUFFER_LENGTH_MAX )
    {
        Uart_RxBuff.u4_WriteCounter = U4_DATA_ZERO;
    }
    u1_ReceiveState = U1_UART_RECEIVE_NOTEMPTY;
}

void LPUART1_RxTx_IRQHandler(void)
{ 
  /*  Call function RXmain in IRQ_UART  */
  Uart_RxMainFunction();
}


void LPUART1_send_char(char data) {
	while((LPUART1->STAT & (1<<23u)) == 0 );
	/* Wait for transmit buffer to be empty */
	LPUART1->DATA = data;              /* Send data */
}

void LPUART1_send_string(char*  data_string )
{  /* Function to Transmit whole string */
	while(*data_string != '\0')  {           /* Send chars one at a time */
		LPUART1_send_char(*data_string);
		data_string++;
	}
}
