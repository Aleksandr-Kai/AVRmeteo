void (*display)();

#define DELAY_LIMIT 50
char delay = 0;

struct {
	float outT;
	float inT;
	float P;
} history[24];
char phistory = 0;

float xP, xT;

#define DELAY_MEAS_T 500
unsigned int delay_meas_t = 1;

#define DELAY_MEAS_P 60000
unsigned int delay_meas_p = 1;

void dispMain() {
	BMP280_S32_t adcP, adcT;
	char i;
	float fp, ft;
	
	if (--delay_meas_t == 0){
		delay_meas_t = DELAY_MEAS_T;
		ft = ds18b20_temperature(0);
		if ((ft > -40) && (ft < 40)) {
			xT = ft;
		}
		
		rtc_time(&hh, &mm, 0, 0);
	}
	
	if (--delay_meas_p == 0){
		delay_meas_p = DELAY_MEAS_P;
		Measure(&adcT, &adcP);
		bmp280_compensate_T_double(adcT);
		fp = bmp280_compensate_P_double(adcP);	
		fp = fp/13.275;
		if ((fp > 7000)&&(fp < 8000)) {
			xP = fp;
		}
	}
	
	if (++delay < DELAY_LIMIT) {
		return;
	}
	delay = 0;
	
	Dyn_Code(digits[hh/10], digits[hh%10], digits[mm/10], digits[mm%10], 0);
	Dyn_Number((unsigned int)xT*10, 1, 1);
	Dyn_Number((unsigned int)xP, 1, 2);

	if ((phistory != hh)&&(mm < 30)) {
		putsf("Update history\r");
		phistory = hh;
		history[phistory].inT = xT;
		history[phistory].P = xP;
	}
}

int timeBlink = 0;
void dispHistory() {
	if (++timeBlink < 400) {
		Dyn_Code(digits[hh/10], digits[hh%10], digits[0], digits[0], 0);
	}else if (timeBlink < 800) {
		Dyn_Code(digits[hh/10], digits[hh%10], DI_null, DI_null, 0);
	}else timeBlink = 0;
	if ((history[hh].inT > -40) && (history[hh].inT < 40)) Dyn_Number((unsigned int)history[hh].inT*10, 1, 1);
	else Dyn_Code(DI_minus, DI_minus, DI_minus, DI_minus, 1);
	if ((history[hh].P < 7000)||(history[hh].P > 8000)) Dyn_Code(DI_minus, DI_minus, DI_minus, DI_minus, 2);
	else Dyn_Number((unsigned int)history[hh].P, 1, 2);
}

void dispMenu() {
	
}

void dispTime() {
	if (++timeBlink < 300) {
		Dyn_Code(digits[hh/10], digits[hh%10], digits[mm/10], digits[mm%10], 0);
	}else if (timeBlink < 500) {
		Dyn_Code(DI_null, DI_null, DI_null, DI_null, 0);
	}else timeBlink = 0;
	Dyn_Code(DI_null, DI_null, DI_null, DI_null, 1);
	Dyn_Code(DI_null, DI_null, DI_null, DI_null, 2);
}