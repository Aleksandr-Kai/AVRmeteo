#ifndef __DYN_DEF__
#define __DYN_DEF__

//=======================================================
//                       Катоды
//    ___________________________________________
//   |    _     |     _    |     _    |     _    |
//   |   |_|    |    |_|   |    |_|   |    |_|   |
//   |   |_|    |    |_|   |    |_|   |    |_|   |
//   |          |          |          |          |
//   |__________|__________|__________|__________|
//  DI_LED1_K1000          DI_LED1_K10
//              DI_LED1_K100           DI_LED1_K1
//=======================================================
#define DI_LED1_K1		PORTB.5  // катоды ПЕРВОГО индикатора
#define DI_LED1_K10		PORTB.6  // * * * * * * * * * * * * *
#define DI_LED1_K100	PORTB.7  // * * * * * * * * * * * * *
#define DI_LED1_K1000	PORTD.7  // * * * * * * * * * * * * *
#define DI_LED1_DASH    PORTA.3

#define DI_LED2_K1		PORTB.1  // катоды ВТОРОГО индикатора  
#define DI_LED2_K10		PORTB.2  // * * * * * * * * * * * * *  
#define DI_LED2_K100	PORTB.3  // * * * * * * * * * * * * *  
#define DI_LED2_K1000	PORTB.4  // * * * * * * * * * * * * *  
	
#define DI_LED3_K1		PORTA.2  // катоды ВТОРОГО индикатора  
#define DI_LED3_K10		PORTA.1  // * * * * * * * * * * * * *  
#define DI_LED3_K100	PORTA.0  // * * * * * * * * * * * * *  
#define DI_LED3_K1000	PORTB.0  // * * * * * * * * * * * * *  

//======================	
//         Aноды
//           A
//         -----
//      F |     | B
//		  |  G  |
//         -----
//      E |     | C
//        |     |
//         ----- * 
//           D     H
//======================
#define DI_A	PORTC.0
#define DI_B	PORTC.1
#define DI_C	PORTC.2
#define DI_D	PORTC.3
#define DI_E	PORTC.4
#define DI_F	PORTC.5
#define DI_G	PORTC.6
#define DI_H	PORTC.7

//==========================================================
//           Дальше ничего не менять
//==========================================================


#define abs(x) ((x<0)? -x : x)
#define true	0xFF
#define false	0x00


//=============================
//           Разряды
//=============================
#define DI_p1		0
#define DI_p10		1
#define DI_p100		2
#define DI_p1000	3


//=============================
//           Индикаторы
//=============================
#define DI_LED1		0
#define DI_LED2		1
#define DI_LED3		2


//=============================
//           Буквы
//=============================
#define DI_code_A	17
#define DI_code_b	193
#define DI_code_C	99
#define DI_code_c	229
#define DI_code_d	133
#define DI_code_E	97
#define DI_code_F	113
#define DI_code_G	67
#define DI_code_H	145
#define DI_code_h	209
#define DI_code_i	223
#define DI_code_J	135
#define DI_code_L	227
#define DI_code_n	213
#define DI_code_o	197
#define DI_code_P	49
#define DI_code_r	245
#define DI_code_S	73
#define DI_code_t	225
#define DI_code_U	131
#define DI_code_u	199


//=============================
//    Специальные символы
//=============================
#define DI_minus 253
#define DI_null 255
#define DI_fail 239
#define DI_dot 254
#define DI_all 0
#define DI_up 0x7F
#define DI_dwn 0xEF

#endif