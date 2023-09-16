#define RECIEVER_SIZE  10
#define TERMINATOR '\r'
#define TRANSMITER_SIZE 10

enum eRecieverStatus {EMPTY, READY, OVERFLOW};
enum eTransmiterStatus {FREE, BUSY}; 
enum eDataReceived {YES, NO};

typedef struct TransmiterBuffer{
	 char cData[TRANSMITER_SIZE]; //data we sending
	 enum eTransmiterStatus eStatus; //state BUSY or FREE
	 enum eDataReceived eData;
	 unsigned char fFirstCharacter;
	 unsigned char fLastCharacter; 
	 unsigned char cCharCtr; 
}sTransmiterBuffer;



void UART_InitWithInt(unsigned int uiBaudRate);
void Reciever_PutCharacterToBuffer(char cCharacter);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
char Transmiter_GetCharacterFromBuffer(void);
void Transmiter_SendString(char cString[]);
enum eTransmiterStatus Transmiter_GetStatus(void);

