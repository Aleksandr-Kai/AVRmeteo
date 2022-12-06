#define PRESS_CNT 10
#define HOLD_CNT 100

#define BTN_PLUS state == 1
#define BTN_MINUS state == 3
#define BTN_MENU state == 2

char btnPlusCount = 0;
void (*onPlusPressed)();
void (*onPlusHold)();

char btnMinusCount = 0;
void (*onMinusPressed)();
void (*onMinusHold)();

char btnMenuCount = 0;
void (*onMenuPressed)();
void (*onMenuHold)();

int prevBtnCode = 0;
int code;
char state = 0;

void ScanButtons()
{
	code = read_adc(4);
	if (code - prevBtnCode > 90 || code - prevBtnCode < -90)
	{
		if (state != 1 && code >= 0 && code < 200)
		{
			state = 1;
		}
		else if (state != 2 && code > 200 && code < 380)
		{
			state = 2;
		}
		else if (state != 3 && code > 380 && code < 480)
		{
			state = 3;
		}
		else if (state != 0 && code > 480)
		{
			state = 0;
		}
	}
	prevBtnCode = code;

	if (BTN_PLUS)
	{
		if (btnPlusCount < HOLD_CNT)
		{
			btnPlusCount++;
		}
		else
		{
			if (onPlusHold != NULL)
				onPlusHold();
		}
	}
	else
	{
		if ((btnPlusCount >= PRESS_CNT) && (btnPlusCount < HOLD_CNT))
		{
			if (onPlusPressed != NULL)
				onPlusPressed();
		}
		btnPlusCount = 0;
	}

	if (BTN_MINUS)
	{
		if (btnMinusCount < HOLD_CNT)
		{
			btnMinusCount++;
		}
		else
		{
			if (onMinusHold != NULL)
				onMinusHold();
		}
	}
	else
	{
		if ((btnMinusCount >= PRESS_CNT) && (btnMinusCount < HOLD_CNT))
		{
			if (onMinusPressed != NULL)
				onMinusPressed();
		}
		btnMinusCount = 0;
	}

	if (BTN_MENU)
	{
		if (btnMenuCount < HOLD_CNT)
		{
			btnMenuCount++;
		}
		else
		{
			if (onMenuHold != NULL)
				onMenuHold();
		}
	}
	else
	{
		if ((btnMenuCount >= PRESS_CNT) && (btnMenuCount < HOLD_CNT))
		{
			if (onMenuPressed != NULL)
				onMenuPressed();
		}
		btnMenuCount = 0;
	}
}