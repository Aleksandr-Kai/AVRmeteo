#ifndef _DYN_INCLUDED_
#define _DYN_INCLUDED_

//=============================
//    !!!! ���������� ��
//=============================
#include <mega16a.h>

#include "Dyn_def.h"

extern unsigned char DI_Digit[4][3];
extern char digits[10];

#pragma used+
void Dyn_Indicate();
void Dyn_Clear(char led);
//void Dyn_Number(int value, char led);
void Dyn_Number(int value, char dot, char led);
void Dyn_Code(unsigned char p1000, unsigned char p100, unsigned char p10, unsigned char p1, unsigned char led);
void Dyn_Mark(char led);
#pragma used-

#pragma library Dyn.c
#endif 