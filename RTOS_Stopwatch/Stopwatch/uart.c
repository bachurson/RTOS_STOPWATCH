#include <LPC210X.H>
#include "uart.h"
#include <string.h>

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020


////////////////////////////RECEIVER////////////////////////////
char cReceivedChar;

struct RecieverBuffer{ 
	char cData[RECIEVER_SIZE]; 
	unsigned char ucCharCtr; 
	enum eRecieverStatus eStatus; 
};
struct RecieverBuffer sBuffer;



void Reciever_PutCharacterToBuffer(char cCharacter){
	if (sBuffer.ucCharCtr > RECIEVER_SIZE - 1){
		sBuffer.eStatus=OVERFLOW;
	}
	else if (cCharacter == TERMINATOR){
		sBuffer.cData[sBuffer.ucCharCtr]='\0';
		sBuffer.eStatus=READY;
		sBuffer.ucCharCtr = 0;
	}
	else{
		sBuffer.cData[sBuffer.ucCharCtr] = cCharacter;
		sBuffer.ucCharCtr+=1;
	}
	
}

enum eRecieverStatus eReciever_GetStatus(void){
	return(sBuffer.eStatus);
}

void Reciever_GetStringCopy(char * ucDestination){
	strcpy(ucDestination, sBuffer.cData);
	sBuffer.eStatus=EMPTY;
}
///////////////////////////////////////////////////////////////////
	

/////////////////////////////TRANSMITER////////////////////////////



sTransmiterBuffer MyTransmiterBuffer;


char Transmiter_GetCharacterFromBuffer(){
	sTransmiterBuffer *buffer;
	buffer = &MyTransmiterBuffer;
	if(buffer->fLastCharacter == 0){
		if(buffer->fFirstCharacter == 1 )
		{
			buffer->fFirstCharacter = 0;
		}
		else
		{
			buffer->cCharCtr++;
		}
		buffer->eStatus = BUSY;
		if(buffer-> cData[buffer->cCharCtr] != '\0'){
			buffer->fLastCharacter = 0;
			return buffer->cData[buffer->cCharCtr];
		}
		else{
			buffer->fLastCharacter = 1;
			return '\r';
		}
	}
	else{
		buffer->cCharCtr = 0;
		buffer->eStatus = FREE;
		buffer->eData = NO;
		buffer->fLastCharacter = 0;
		buffer->fFirstCharacter = 1;
		return '\0';
	}
}

void Transmiter_SendString(char cString[]){
	sTransmiterBuffer *buffer;
	buffer = &MyTransmiterBuffer;
	strcpy(buffer->cData, cString);
	buffer->eStatus = BUSY;
	buffer->eData = YES;
	buffer->cCharCtr = 0;	
	U0THR = buffer->cData[0];
	buffer->fFirstCharacter = 0;
}

enum eTransmiterStatus Transmiter_GetStatus(void){
	sTransmiterBuffer *buffer;
	buffer = &MyTransmiterBuffer;
	return buffer->eStatus;
}
	
//////////////////////////////////////////////////////////////////

__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii
	 sTransmiterBuffer *buffer;
	 buffer = &MyTransmiterBuffer;

   if((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
     // cReceivedChar = U0RBR;
		  //Reciever_PutCharacterToBuffer(cReceivedChar);
   } 
   
   if (((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING) && (buffer->eData == YES) )              // wyslano znak - nadajnik pusty 
   {
       U0THR = Transmiter_GetCharacterFromBuffer();
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}

/////////////////////////////////////////////////////////////////

void UART_InitWithInt(unsigned int uiBaudRate){
	
	unsigned long ulDivisor, ulWantedClock;
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock;
	// UART
	PINSEL0 = PINSEL0 | 0x55;                                     // ustawic piny uar0 odbiornik nadajnik
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // d³ugosc s³owa, DLAB = 1
	U0DLL = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	ulDivisor >>= 8;
	U0DLM = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;               

	// INT
	VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel

}

