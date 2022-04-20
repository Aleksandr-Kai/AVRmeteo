#define PRESS_CNT 10
#define HOLD_CNT 100

#define BTN_PLUS		!PINA.4
#define BTN_MINUS	!PINA.6
#define BTN_MENU		!PINA.5

char btnPlusCount = 0;
void (*onPlusPressed)();
void (*onPlusHold)();

char btnMinusCount = 0;
void (*onMinusPressed)();
void (*onMinusHold)();

char btnMenuCount = 0;
void (*onMenuPressed)();
void (*onMenuHold)();

void ScanButtons() {
	if (BTN_PLUS){
		if(btnPlusCount < HOLD_CNT) {
			btnPlusCount++;
		}else{
			if (onPlusHold != NULL) onPlusHold();
		}
	}else{
		if((btnPlusCount >= PRESS_CNT)&&(btnPlusCount < HOLD_CNT)) {
			if (onPlusPressed != NULL) onPlusPressed();
		}
		btnPlusCount = 0;
	}
	
	if (BTN_MINUS){
		if(btnMinusCount < HOLD_CNT) {
			btnMinusCount++;
		}else{
			if(onMinusHold != NULL) onMinusHold();
		}
	}else{
		if((btnMinusCount >= PRESS_CNT)&&(btnMinusCount < HOLD_CNT)) {
			if(onMinusPressed != NULL) onMinusPressed();
		}
		btnMinusCount = 0;
	}
	
	if (BTN_MENU){
		if(btnMenuCount < HOLD_CNT) {
			btnMenuCount++;
		}else{
			if(onMenuHold != NULL) onMenuHold();
		}
	}else{
		if((btnMenuCount >= PRESS_CNT)&&(btnMenuCount < HOLD_CNT)) {
			if(onMenuPressed != NULL) onMenuPressed();
		}
		btnMenuCount = 0;
	}
}