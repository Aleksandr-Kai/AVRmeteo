#define PRESS_CNT 10
#define HOLD_CNT 100

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

void ScanButtons()
{
	code = read_adc(4);
	if (code >= 0 && code < 200)
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
	else if (code > 200 && code < 380)
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
	else if (code > 380 && code < 480)
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

		if ((btnMinusCount >= PRESS_CNT) && (btnMinusCount < HOLD_CNT))
		{
			if (onMinusPressed != NULL)
				onMinusPressed();
		}
		btnMinusCount = 0;

		if ((btnMenuCount >= PRESS_CNT) && (btnMenuCount < HOLD_CNT))
		{
			if (onMenuPressed != NULL)
				onMenuPressed();
		}
		btnMenuCount = 0;
	}

	prevBtnCode = code;
}