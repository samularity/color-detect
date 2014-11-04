/*
 * Created: 30.10.2013 22:08:51
 *  Author: Samuel Munz
 */ 

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PinOut TinyMega :	http://shackspace.de/wiki/doku.php?id=project:tinymega

F0 - Analog in LDR
B0 - Red
B1 - Green
B2 - Blue

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "main.h"
#include "adc.h"
#include "color_detect.h"
#include "uart.h"

#define BAUD_RATE 9600

int main(void)
{
	clock_prescale_set(clock_div_1);	//clock prescaler auf  1 =>  16MHz
	MCUCR = (1 << JTD);					//jtag ausschalten
	
	DDRE &= ~(1 << PE2); // pin d2 als eingang
	PORTE |= (1 << PE2); // pullup aktiviern
	DDRD|=(1 << PD3); //uart tx
	DDRE|=(1 << PE6); //board led
	DDRB = 0xff;//alle als ausgang für leds
	PORTB= 0xFF;//alle als ausgang für leds
	DDRF = 0x00; //alle eingang für LDR
	
	adc_init();
	usb_init();			//initalisiere usb
	
	
	PORTB= 0x00;
	sleep_ms(125);
	PORTB= 0xFF;
	sleep_ms(125);
	PORTB= 0x00;
	sleep_ms(125);
	PORTB= 0xFF;
	
	
	uart_init(BAUD_RATE);
	uart_putstr("\r\ninit done");
	sleep_ms(500);
	
while(1)   
 {
		if ( !(PINE & (1 << PE2)) ) //portpin auf gnd
		{
			sleep_ms(50);
			usb_send_int(readADC(0));
			usb_send_str("\r\n");
		}
	 
	if (usb_serial_available())
	 {	
		char input =usb_serial_getchar();
		usb_serial_putchar(input);
		if (input=='q'||input==27){bootloader();}
			
		switch(input)
		{
		case 's': sw_abgleich(schwarz);break;
		case 'w': sw_abgleich(weiss);break;
		case 'q': 
		case 27: E6_on; bootloader(); break;
		default: color_task();break;
		}	
		usb_serial_flush_input();	
	 }
	//   usb_serial_putchar(input);

 }//while
}//main

void sleep_us(uint16_t us){
	while(us){
		us--;
		_delay_us(1);
	}
}
void sleep_ms(uint16_t ms){
	while(ms){
		ms--;
		_delay_ms(1);
	}
}

void bootloader (void)
{
	MCUCR |=  (1 << IVCE);  //IVCE  = 1		//register für restret
	MCUCR |=  (1 << IVSEL); //IVSEL = 1		//register für restret
	TIMSK0 = 0;             //Timer-Interrupt ausschalten
	sleep_ms(25);
	asm volatile ("jmp 0x3800");
}
