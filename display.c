void (*display)();

#define DELAY_LIMIT 100
char delay = 0;

struct {
	float outT;
	float inT;
	float P;
} history[24];
char phistory = 0;

#define ARR_T_SIZE 200
#define DELAY_MEAS_T 50
unsigned int delay_meas_t = DELAY_MEAS_T;
float arrT[ARR_T_SIZE];
char p_arrT = 0;
float avT = 0;

#define ARR_P_SIZE 200
#define DELAY_MEAS_P 300
unsigned int delay_meas_p = DELAY_MEAS_P;
float arrP[ARR_P_SIZE];
char p_arrP = 0;
float avP = 0;

//=========================================================
//                      ShowTemp()
//                 Отображение теипературы
//=========================================================
void ShowTemp()
{
	int tmp;
	
	//рисуем маркер комнатного датчика
	if(sensor) DI_Digit[DI_p1000][DI_LED2] = DI_code_H; else DI_Digit[DI_p1000][DI_LED2] = DI_null;
	
	//ругаемся на запредельную температуру
	if((avT < -99)||(avT > 99)){
		DI_Digit[0][1] = DI_minus;
		DI_Digit[1][1] = DI_minus;
		DI_Digit[2][1] = DI_minus;
		DI_Digit[3][1] = DI_minus;
		return;
	}
	if(avT == 0){//если температура нулевая, ноль и рисуем, без проверки разрядов и знака
		DI_Digit[DI_p1][DI_LED2] = digits[0];
		DI_Digit[DI_p10][DI_LED2] = DI_null;
		DI_Digit[DI_p100][DI_LED2] = DI_null;
	}else{//иначе ищем где рисовать минус, и где стирать лишние нули
		tmp = (int)(temp[sensor]*10);//так надо
		if(tmp < 0) tmp = tmp * -1;
		
		DI_Digit[DI_p1][DI_LED2] = digits[tmp%10];
		DI_Digit[DI_p10][DI_LED2] = digits[(tmp/10)%10] & DI_dot;
		DI_Digit[DI_p100][DI_LED2] = digits[(tmp/100)%10];
		
		if(temp[sensor]<0){//для отрицательной температуры выбираем разряд под отображение минуса
			if(DI_Digit[DI_p100][DI_LED2] == digits[0]){
				DI_Digit[DI_p100][DI_LED2] = DI_minus;
			}else{
				DI_Digit[DI_p1000][DI_LED2] = DI_minus;
			}
		}else if(DI_Digit[DI_p100][DI_LED2] == digits[0]) DI_Digit[DI_p100][1] = DI_null;//для положительной темп. стираем лишние нули
	}
}

void dispMain() {
	BMP280_S32_t P, T;
	char i;
	float fp, ft;
	
	if (--delay_meas_t == 0){
		delay_meas_t = DELAY_MEAS_T;
		ft = ds18b20_temperature(0);
		if ((ft > -40) && (ft < 40)) {
			arrT[p_arrT] = ft;
			if (++p_arrT == ARR_T_SIZE) p_arrT = 0;
			ft = 0;
			for (i = 0; i < ARR_T_SIZE; i++) {
				ft += arrT[i];
			}
			avT = ft / ARR_T_SIZE;
		}
	}
	
	if (--delay_meas_p == 0){
		delay_meas_p = DELAY_MEAS_P;
		Measure(&T, &P);
		ft = bmp280_compensate_T_double(T);
		fp = bmp280_compensate_P_double(P);	
		fp = fp/13.275;
		if ((fp > 7000)&&(fp < 8000)) {
			arrP[p_arrP] = fp;
			if (++p_arrP == ARR_P_SIZE) p_arrP = 0;
			fp = 0;
			for (i = 0; i < ARR_P_SIZE; i++) {
				fp += arrP[i];
			}
			avP = fp / ARR_P_SIZE;
		}
	}
	
	if (++delay < DELAY_LIMIT) {
		return;
	}
	delay = 0;
	
	rtc_time(&hh, &mm, 0, 0);
	Dyn_Code(digits[hh/10], digits[hh%10], digits[mm/10], digits[mm%10], 0);
	
	//avT = ds18b20_temperature(0);
	ShowTemp();
	Dyn_Number((unsigned int)avP, 1, 2);
	/*
	Measure(&T, &P);
	ft = bmp280_compensate_T_double(T);
	fp = bmp280_compensate_P_double(P);	
	fp = fp/13.275;
	if ((fp < 7000)||(fp > 8000)) {
		if (failP) Dyn_Code(DI_minus, DI_minus, DI_minus, DI_minus, 2);
		else failP = 1;
	}else {
		Dyn_Number((unsigned int)fp, 1, 2);
		failP = 0;
	}*/
	if (phistory != hh) {
		putsf("Update history\r");
		phistory = hh;
		history[phistory].inT = temp[sensor];
		history[phistory].P = fp;
	}
}

int timeBlink = 0;
void dispHistory() {
	if (++timeBlink < 400) {
		Dyn_Code(digits[hh/10], digits[hh%10], digits[0], digits[0], 0);
	}else if (timeBlink < 800) {
		Dyn_Code(digits[hh/10], digits[hh%10], DI_null, DI_null, 0);
	}else timeBlink = 0;
	avT = history[hh].inT;
	ShowTemp();	
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
	//Dyn_Code(DI_null, DI_null, DI_null, DI_null, 1);
	//Dyn_Code(DI_null, DI_null, DI_null, DI_null, 2);
	Dyn_Code(DI_null, DI_dwn, DI_minus, DI_up, 1);
	Dyn_Code(DI_null, DI_up, DI_minus, DI_dwn, 2);
}