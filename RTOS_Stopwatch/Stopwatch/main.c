#include "stopwatch.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "keyboard.h"
#include "semphr.h"



int main( void ){
	xSemaphoreHandle xSemaphore;
	UART_InitWithInt(300);
	Keyboard_Init();
	vSemaphoreCreateBinary( xSemaphore );
	xTaskCreate(TimerTx, NULL, 128, &xSemaphore, 1, NULL );
	xTaskCreate(CountStop, NULL, 128, &xSemaphore, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
