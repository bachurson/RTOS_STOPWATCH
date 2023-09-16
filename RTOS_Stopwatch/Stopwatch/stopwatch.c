#include "stopwatch.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "uart.h"
#include "keyboard.h"
#include <stdio.h>

unsigned char minutes = 0;
unsigned char seconds = 0;
unsigned char hours = 0;
char time_char[9];

TickType_t xLastWakeTime;
const TickType_t xWaitTime = pdMS_TO_TICKS( 1000 );

void Rtos_Transmiter_SendString(char cString[9], xSemaphoreHandle *xSemaphore){
		 xSemaphoreTake(*xSemaphore, portMAX_DELAY);
		 while(Transmiter_GetStatus() == BUSY){}
		 Transmiter_SendString(cString);
			 
		 while (Transmiter_GetStatus() == BUSY){}
		 xSemaphoreGive(*xSemaphore);
}

void TimerTx (void *pvParameters){
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	xLastWakeTime = xTaskGetTickCount();
	while(1){
	
		seconds++;

		minutes += seconds / 60;
		seconds %= 60;

		hours += minutes / 60;
		minutes %= 60;

		hours %= 24;

		sprintf(time_char, "%02d:%02d:%02d", hours, minutes, seconds);
		vTaskDelayUntil( &xLastWakeTime, xWaitTime );
		xLastWakeTime = xTaskGetTickCount();
		Rtos_Transmiter_SendString(time_char, xSemaphore);
		

	}
} 

void CountStop (void *pvParameters){
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1){
		
		if(eRead() != RELEASED)
		{
			xSemaphoreTake(*xSemaphore, portMAX_DELAY);
			vTaskDelay(500);
			while(eRead() == RELEASED){}
			vTaskDelay(500);
			xLastWakeTime = xTaskGetTickCount();
		  xSemaphoreGive(*xSemaphore);
		}
		else{}
	}
} 

