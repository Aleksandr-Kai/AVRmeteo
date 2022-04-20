#include "Dyn.h"

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
	// Reinitialize Timer 0 value
	TCNT0=0xF0;
	//Dyn_Indicate();
}