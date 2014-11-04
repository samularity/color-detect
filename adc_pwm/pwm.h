/*
 * pwm.h
 *
 * Created: 04.11.2014 11:27:16
 *  Author: Samuel
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

#define PWM_LED_RED		OCR1A //pb5
#define PWM_LED_GREEN	OCR1B //pb6
#define PWM_LED_BLUE	OCR1C //pb7


#define RED		PWM_LED_RED=4095;PWM_LED_GREEN=0;PWM_LED_BLUE=0;
#define GREEN	PWM_LED_RED=0;PWM_LED_GREEN=4095;PWM_LED_BLUE=0;
#define BLUE	PWM_LED_RED=0;PWM_LED_GREEN=0;PWM_LED_BLUE=4095;


void timer1_init(void);

#endif /* PWM_H_ */