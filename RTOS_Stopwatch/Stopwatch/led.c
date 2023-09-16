#include "led.h"
#include <LPC21xx.H>

#define LED_COUNT 4
const int LED_bm[LED_COUNT] = {1<<16, 1<<17, 1<<18, 1<<19};

void Led_Init()
{
	IO1DIR=IO1DIR|(LED_bm[0]|LED_bm[1]|LED_bm[2]|LED_bm[3]);
}

void Led_Toggle(unsigned char ucLedIndex)
{
	IO1CLR=(LED_bm[0]|LED_bm[1]|LED_bm[2]|LED_bm[3]) & (~LED_bm[ucLedIndex]);
	switch(ucLedIndex)
	{
		case 0:
			if((IO1PIN & LED_bm[0])  == 0){
				IO1SET  = LED_bm[0];
			}
			else {
				IO1CLR = LED_bm[0];
			}
			break;
		case 1:
			if((IO1PIN & LED_bm[1]) == 0){
				IO1SET = LED_bm[1];
			}
			else {
				IO1CLR = LED_bm[1];
			}
			break;
		case 2:
			if((IO1PIN & LED_bm[2]) == 0){
				IO1SET = LED_bm[2];
			}
			else {
				IO1CLR = LED_bm[2];
			}
			break;
		case 3:
			if((IO1PIN & LED_bm[3]) == 0){
				IO1SET = LED_bm[3];
			}
			else {
				IO1CLR = LED_bm[3];
			}
			break;
		default:
			break;
	}
}



