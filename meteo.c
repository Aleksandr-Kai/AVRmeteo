/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 29.03.2022
Author  : 
Company : 
Comments: 


Chip type               : ATmega16A
Program type            : Application
AVR Core Clock frequency: 16,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16a.h>
#include <delay.h>
#include <meteo_conf.c>
#include "rc3231.c"
#include "bmp180.c"

#define MODE_TIMEOUT 5000

float temp[2], f_Tmp;
int i_Tmp; 
unsigned char sensor, latch, temp_read;
char mm, hh;
char buff[100];

void setMain();
void terminator() {}
// ***********************************************************
//                              Timeout
// ***********************************************************
void (*timeoutHandler)();
int timeoutCounter = -1;
int timeoutStore = -1;
void setTimeout(int duration, void (*handler)()) {
	timeoutCounter = duration;
	timeoutStore = duration;
	timeoutHandler = handler;
}
void timeoutReset(){
	timeoutCounter = timeoutStore;
}
// ***********************************************************
#include "display.c"
#include "buttons.c"

interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
	TCNT2=0x64;
	ScanButtons();
}

void hourPlus(){
	if(++hh > 23) hh = 0;
	timeoutReset();
}

void hourMinus(){
	if(--hh > 23) hh = 23;
	timeoutReset();
}

void histHourPlus(){
	timeoutReset();
	if (hh == phistory) return;
	if(++hh > 23) hh = 0;
}

void histHourMinus(){
	char h;
	timeoutReset();
	h = hh-1;
	if (h > 23) h = 23;
	if (h == phistory) return;
	if(--hh > 23) hh = 23;
}

char hdelay = 0;
void hourHoldPlus(){
	if(++hdelay < 20) return;
	hdelay = 0;
	if(++hh > 23) hh = 0;
	timeoutReset();
}

void hourHoldMinus(){
	if(++hdelay < 20) return;
	hdelay = 0;
	if(--hh > 23) hh = 23;
	timeoutReset();
}

void minutPlus() {
	if(++mm > 59) mm = 0;
	timeoutReset();
}

void minutHoldPlus() {
	if(++hdelay < 10) return;
	hdelay = 0;
	if(++mm > 59) mm = 0;
	timeoutReset();
}

void updateTime() {
	putsf("Update time\r");
	//rtc_set_time(hh, mm, 0, 0);
	//setMain();
	display = &terminator;
	Dyn_Code(digits[hh/10], digits[hh%10], digits[mm/10], digits[mm%10], 0);
	Dyn_Code(DI_null, DI_code_S, DI_code_E, DI_code_t, 1);
}

void setTime() {
	putsf("Set Time\r");
	blinkDots = 0;
	
	display = &dispTime;
	onPlusPressed = &hourPlus;
	onMinusPressed = &minutPlus;
	onPlusHold = &hourHoldPlus;
	onMinusHold = &minutHoldPlus;
	onMenuPressed = &updateTime;
	setTimeout(MODE_TIMEOUT, &setMain);
}

void setHistory() {
	putsf("Set History\r");
	blinkDots = 0;
	display = &dispHistory;
	onPlusPressed = &histHourPlus;
	onMinusPressed = &histHourMinus;
	onPlusHold = &terminator;
	onMinusHold = &terminator;
	onMenuPressed = &setMain;
	setTimeout(MODE_TIMEOUT, &setMain);
}

void setMenu() {
	putsf("Set Menu\r");
}

void setMain() {
	putsf("Set Main\r");
	blinkDots = 1;
	display = &dispMain;
	onPlusPressed = &setHistory;
	onMinusPressed = &setHistory;
	onMenuPressed = &setTime;
}

void main(void) {
	char i;
	chipinit();

	sensor = 0;
	//rtc_set_time(11, 59, 0, 0);
	BMP180_Calibration();

	for(i = 0; i < 24; i++) {
		history[i].inT = 255;
		history[i].P = 0;
	}
	putsf("\033c");
	setMain();
	
	while (1) {
		display();
		if (timeoutCounter > 0) {
			if (--timeoutCounter == 0) {
				timeoutHandler();
			}
		}
		delay_ms(1);
	}
}
