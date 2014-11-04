/*
 * pwm.c
 *
 * Created: 04.11.2014 11:23:48
 *  Author: Samuel
 */
#include "pwm.h"


void timer1_init(void){
	
	PWM_LED_RED		= 2048;
	PWM_LED_GREEN	= 2048;
	PWM_LED_BLUE	= 2048;
	
	DDRB |= (1<<DDB7)|(1<<DDB6)|(1<<DDB5);
	//Modus und Vorteiler wählen (Phase Correct PWM und Vorteiler 8)
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<COM1C1)|(1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<CS11);
	//Auflösung 12-Bit
	ICR1 |= 0x0FFF;
}