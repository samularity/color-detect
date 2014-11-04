/*
 * main.h
 *
 * Created: 10.12.2013 14:49:08
 *  Author: Samuel
 */ 

#ifndef _main_h_
#define _main_h_

#include <avr/io.h>
#include <avr/power.h> //für set prescaler
#define F_CPU 16000000UL  //muss vor delay
#include <util/delay.h> //_delay_ms

#include <stdlib.h>	//utoa
#include <stdbool.h>//erlaubt bool
#include "usb_serial.h"

//defines
#define E6_on PORTE |= (1<<PINE6);//debug led an
#define E6_off PORTE &= ~(1<<PINE6);//debug led aus

//funktionen
void sleep_us(uint16_t us);
void sleep_ms(uint16_t ms);
void bootloader (void);

#endif
