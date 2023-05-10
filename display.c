void (*display)();

#define DELAY_LIMIT 50
char delay = 0;

struct
{
	int outT;
	int inT;
	int P;
} history[24];
char phistory = 255;

#define DELAY_SWITCH_T 3000
unsigned int delay_switch_t = 1;

#define DELAY_MEAS_T 500
unsigned int delay_meas_t = 1;

#define DELAY_MEAS_P 60000
unsigned int delay_meas_p = 1;

#define PRESSURE_LIMIT_UP 800
#define PRESSURE_LIMIT_DOWN 700
#define TEMP_LIMIT 50
#define TIME_LED 0
#define INNER_LED 1
#define OUTER_LED 2
#define TEMP_FAIL_LIMIT 20

char showPressure = 0;
int fp, ft;
int lastValid_ft = 0;
char ft_valid_cnt = TEMP_FAIL_LIMIT;
// char outcnt = 0;

int lastUpdateOuterSensor = 0;

#define OUTER_SENSOR_TIMEOUT 15
// #define OUTER_SENSOR_FAIL outcnt == OUTER_SENSOR_TIMEOUT

char isCorrectTemp(int t)
{
	t /= 10;
	return (t > -TEMP_LIMIT) && (t < TEMP_LIMIT);
}

char isCorrectPressure(int p)
{
	p /= 10;
	return (p > PRESSURE_LIMIT_DOWN) && (p < PRESSURE_LIMIT_UP);
}

char OuterSensorFail(char hh, char mm) {
	// char buff[128];
	int minutes,dm;
	if (lastUpdateOuterSensor < 0) return 1;
	minutes = (int)hh * 60 + mm;
	dm = minutes - lastUpdateOuterSensor;

	// sprintf(buff, "%i->%i, %i", lastUpdateOuterSensor, minutes, dm);
	// puts(buff);

	if (dm >= OUTER_SENSOR_TIMEOUT) {
		lastUpdateOuterSensor = -1;
		outtemp = TEMP_LIMIT*10;
		return 1;
	}

	return 0;
}

void updateOuterSensorTimeout(char hh, char mm){
	lastUpdateOuterSensor = (int)hh * 60 + mm;
}

void dispMain()
{
	BMP280_S32_t adcP, adcT;

	if (--delay_switch_t == 0)
	{
		
		showPressure = !showPressure;
		if(showPressure) 
			delay_switch_t = DELAY_SWITCH_T * 3;
		else
			delay_switch_t = DELAY_SWITCH_T;
		// if (!showPressure && !outerTempUpdated)
		// {
		// 	if (++outcnt > OUTER_SENSOR_TIMEOUT)
		// 		outcnt = OUTER_SENSOR_TIMEOUT;
		// }
	}

	if (--delay_meas_t == 0)
	{
		delay_meas_t = DELAY_MEAS_T;
		ft = ds18b20_temperature(0) * 10;
		if (isCorrectTemp(ft))
		{
			lastValid_ft = ft;
			ft_valid_cnt = 0;
		}
		else if (++ft_valid_cnt > TEMP_FAIL_LIMIT)
		{
			ft_valid_cnt = TEMP_FAIL_LIMIT;
		}
		rtc_time(&hh, &mm, 0, 0);
	}

	if (--delay_meas_p == 0)
	{
		delay_meas_p = DELAY_MEAS_P;
		Measure(&adcT, &adcP);
		bmp280_compensate_T_double(adcT);
		fp = bmp280_compensate_P_double(adcP) / 13.275;
	}

	if (++delay < DELAY_LIMIT)
	{
		return;
	}
	delay = 0;

	// display TIME
	Dyn_Code(digits[hh / 10], digits[hh % 10], digits[mm / 10], digits[mm % 10], TIME_LED);

	// select current data for INNER_LED
	if (showPressure)
	{
		// display PRESSURE
		if (isCorrectPressure(fp))
			Dyn_Number(fp, 1, INNER_LED);
		else
			Dyn_Code(DI_minus, DI_minus, DI_minus, DI_code_P, INNER_LED);
	}
	else
	{
		// display INNER TEMPERATURE
		if (ft_valid_cnt < TEMP_FAIL_LIMIT)
			Dyn_Number(lastValid_ft, 1, INNER_LED);
		else
			Dyn_Code(DI_minus, DI_minus, DI_minus, DI_minus, INNER_LED);
	}

	// check outer t sensor
	if (outerTempUpdated)
	{
		// outcnt = 0;
		outerTempUpdated = 0;
		if(isCorrectTemp(outtemp))
			updateOuterSensorTimeout(hh, mm);
	}

	// display OUTER TEMPRETURE
	if (OuterSensorFail(hh, mm) || !isCorrectTemp(outtemp))
	{
		Dyn_Code(DI_minus, DI_minus, DI_minus, DI_code_t, OUTER_LED);
	}
	else
		Dyn_Number(outtemp, 1, OUTER_LED);

	// write HISTORY
	if ((phistory != hh) && (mm < 30))
	{
		phistory = hh;
		history[phistory].inT = lastValid_ft;
		history[phistory].outT = outtemp;
		history[phistory].P = fp;
	}
	else if (!isCorrectTemp(history[phistory].outT) && (mm < 30))
	{
		if (OuterSensorFail(hh, mm) || !isCorrectTemp(outtemp))
			history[phistory].outT = TEMP_LIMIT * 10;
		else
			history[phistory].outT = outtemp;
	}
}

int timeBlink = 0;
void dispHistory()
{
	if (++timeBlink < 400)
	{
		// Dyn_Code(digits[hh / 10], digits[hh % 10], digits[0], digits[0], TIME_LED);
		Dyn_Code(digits[hh / 10], digits[hh % 10], DI_null, DI_code_H, TIME_LED);
	}
	else if (timeBlink < 800)
	{
		Dyn_Code(digits[hh / 10], digits[hh % 10], DI_null, DI_null, TIME_LED);
	}
	else
		timeBlink = 0;


	if (--delay_switch_t == 0)
	{
		delay_switch_t = DELAY_SWITCH_T / 3;
		showPressure = !showPressure;
	}

	if (isCorrectTemp(history[hh].outT))
		Dyn_Number(history[hh].outT, 1, OUTER_LED);
	else
		Dyn_Code(DI_minus, DI_minus, DI_minus, DI_code_t, OUTER_LED);

	if (showPressure)
	{
		if (!isCorrectPressure(history[hh].P))
			Dyn_Code(DI_minus, DI_minus, DI_minus, DI_code_P, INNER_LED);
		else
			Dyn_Number(history[hh].P, 1, INNER_LED);
	}
	else
	{
		if (isCorrectTemp(history[hh].inT))
			Dyn_Number(history[hh].inT, 1, INNER_LED);
		else
			Dyn_Code(DI_minus, DI_minus, DI_minus, DI_minus, INNER_LED);
	}
}

void dispTime()
{
	if (++timeBlink < 300)
	{
		Dyn_Code(digits[hh / 10], digits[hh % 10], digits[mm / 10], digits[mm % 10], TIME_LED);
	}
	else if (timeBlink < 500)
	{
		Dyn_Code(DI_null, DI_null, DI_null, DI_null, TIME_LED);
	}
	else
		timeBlink = 0;
	Dyn_Code(DI_null, DI_null, DI_null, DI_null, INNER_LED);
	Dyn_Code(DI_null, DI_null, DI_null, DI_null, OUTER_LED);
}

void dispVersion()
{
	char b = build;
	if (b < 10)
		b *= 10;
	Dyn_Code(DI_code_U, DI_code_E, DI_code_r, DI_code_S, TIME_LED);
	Dyn_Code(DI_code_H, DI_null, DI_null, digits[hw], INNER_LED);
	Dyn_Code(DI_code_F, digits[fw] & DI_dot, digits[b / 10], digits[b % 10], OUTER_LED);
}

void dispPhone()
{
	Dyn_Number(7909, 0, TIME_LED);
	Dyn_Number(407, 0, INNER_LED);
	Dyn_Number(3167, 0, OUTER_LED);
}