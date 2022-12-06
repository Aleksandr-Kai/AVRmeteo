#define RTC_AM	0x10
#define RTC_PM	0x11
#define RTC_24		0x01
#define RTC_W		0xD0
#define RTC_R		0xD1

#define EE_W	0xA0
#define EE_R	0xA1

void rtc_time(char* hh, char* mm, char* ss, char* pm) {
	char data = 0;

	i2c_start();    // запуск i2c
	i2c_write(RTC_W);    // передача адреса устройства, режим записи
	i2c_write(0);   // передача адреса памяти 
	i2c_stop();    // остановка i2c

	i2c_start();    // запуск i2c
	i2c_write(RTC_R);    // передача адреса устройства, режим чтения

	if (ss) {
		data = i2c_read(1);
		*ss = (data >> 4) * 10 + (data & 0x0F);
	}else
		i2c_read(1);

	if (mm) {
		data = i2c_read(1);
		*mm = (data >> 4) * 10 + (data & 0x0F);
	}else
		i2c_read(1);

	if (hh) {
		data = i2c_read(0);
		*hh = ((data >> 4) & 1) * 10 + (data & 0x0F);
		
		if (pm) {
			*pm = data >> 5;
		}
	}else
		i2c_read(0);

	i2c_stop();
}

void rtc_set_time(char hh, char mm, char ss, char pm) {
	if (pm > 1) {
		pm = 1;
	}
	i2c_start();    
	i2c_write(RTC_W);   
	i2c_write(0);  
	i2c_write(((ss/10)<<4) | (ss%10));
	i2c_write(((mm/10)<<4) | (mm%10)); 
	i2c_write((0x80 | (pm << 5) | (hh/10)<<4) | (hh%10)); 
	i2c_stop();   
}
/*

void ee_write(unsigned int addr, char* data, unsigned int length) {
	i2c_start();    
	i2c_write(EE_W);
	i2c_write((addr >> 8) & 0x0F);
	i2c_write(addr & 0xFF);
	for (; length > 0; length--) {
		i2c_write(*data);  
		data++;
	}
	i2c_stop();  
}

void ee_read(unsigned int addr, char* data, unsigned int length) {
	i2c_start();    
	i2c_write(EE_W);   
	i2c_write((addr >> 8) & 0x0F);  
	i2c_write(addr & 0xFF);
	i2c_write(EE_R); 
	i2c_read(1);
	i2c_stop();
	
	i2c_start();
	i2c_write(EE_R); 
	for (; length > 0; length--) {
		*data = i2c_read(0);  
		data++;
	}
	i2c_stop();   
}

void ee_write_byte(unsigned int addr, char data) {
	i2c_start();    
	i2c_write(EE_W);
	i2c_write((addr >> 8) & 0x0F);
	i2c_write(addr & 0xFF);
	i2c_write(data);  
	i2c_stop();
}

char ee_read_byte(unsigned int addr){
	char data;
	i2c_start();    
	i2c_write(EE_W);   
	i2c_write((addr >> 8) & 0x0F);  
	i2c_write(addr & 0xFF);
	i2c_write(EE_R); 
	data = i2c_read(1);  
	i2c_stop();   
	return data;
}*/