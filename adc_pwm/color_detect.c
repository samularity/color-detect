/*
 * color_detect.c
 *
 *  Created on: 21.10.2014
 *      Author: Samuel
 */

#include "color_detect.h"
#include "adc.h"
#include "usb_serial.h"

#define LED_OFF(s)  PORTB |=  (1<<s);
#define LED_ON(s)   PORTB &= ~(1<<s);

#define LED_RED   0
#define LED_GREEN 1
#define LED_BLUE  2

uint16_t get_color_value (uint8_t color); //turn on led, measure an off again
uint16_t calulate_color_value (uint16_t color_val, uint16_t black_val, uint16_t div_val);//wert-schwart/diff

//werte mit vorkalibrierung
// weiﬂwert (max wert)
uint32_t red_white		=875;
uint32_t green_white	=925;
uint32_t blue_white		=895;
// schwarzwert (minimaler wert)
uint32_t red_black		=580;
uint32_t green_black	=770;
uint32_t blue_black		=720;
//differenzwert (wertebereich)
uint32_t red_diff		=295;
uint32_t green_diff		=155;
uint32_t blue_diff		=175;


void color_task (void)
{
	LED_OFF(LED_RED);
	LED_OFF(LED_GREEN);
	LED_OFF(LED_BLUE);
	usb_send_str("\r\n\r\nMessung:");
	sleep_ms(125);

	uint16_t red,green,blue=0;

	red		= get_color_value(LED_RED);
	green	= get_color_value(LED_GREEN);
	blue	= get_color_value(LED_BLUE);

	usb_send_str("\r\nRED:\t");
	usb_send_int(red);
	usb_send_str("\r\nGREEN:\t");
	usb_send_int(green);
	usb_send_str("\r\nBLUE:\t");
	usb_send_int(blue);

	red= calulate_color_value(red,red_black,red_diff);
    green= calulate_color_value(green,green_black,green_diff);
    blue= calulate_color_value(blue,blue_black,blue_diff);

	usb_send_str("\r\n\r\nRED:\t");
	usb_send_int(red);
	usb_send_str("\r\nGREEN:\t");
	usb_send_int(green);
	usb_send_str("\r\nBLUE:\t");
	usb_send_int(blue);

	if (red>blue && red>green){LED_ON(LED_RED);}//red
	else if (blue>green){LED_ON(LED_BLUE);}//blue
	else{LED_ON(LED_GREEN);}//green
}

uint16_t calulate_color_value (uint16_t color_val, uint16_t black_val, uint16_t div_val )//wert-schwart/diff
{
	usb_send_str("\r\ncol,black,div\t");
    usb_send_int(color_val);
	usb_send_str("|");
	usb_send_int(black_val);
	usb_send_str("|");
	usb_send_int(div_val);
	
	usb_send_str("\r\n255*(color_val-black_val)\t");
	usb_send_int(2);
	float temp= 255*(color_val-black_val);
	usb_send_int((uint32_t)temp);
	
	temp= temp/div_val;
	usb_send_str("\r\ndiv_val");
	usb_send_int((uint32_t)temp);

	float ret_val = 255*(color_val-black_val)/div_val;
  return ((uint16_t)temp);
}

uint16_t get_color_value (uint8_t led_nr)
{
	uint16_t ret_val =0;
	LED_ON(led_nr);
	sleep_ms(250);
	ret_val= readADC(0);
	LED_OFF(led_nr);
	sleep_ms(100);
	return ret_val;
}

void sw_abgleich (uint8_t sw)//weiﬂ=0; schwarz=1
{
if (sw == weiss)//weiﬂabgleich
	{
		usb_send_str("\r\nWeiﬂabgleich startet in 3sek \r\n");
		for (uint8_t i =0;i<8;i++)
		{
		red_white  += get_color_value(LED_RED);
		green_white+= get_color_value(LED_GREEN);
		blue_white += get_color_value(LED_BLUE);
		}
		red_white   /= 8;
		green_white /= 8;
		blue_white  /= 8;

			usb_send_str("\r\nWeiﬂabgleich\r\nRED:\t");
			usb_send_int(red_white);
			usb_send_str("\r\nGREEN:\t");
			usb_send_int(green_white);
			usb_send_str("\r\nBLUE:\t");
			usb_send_int(blue_white);
	}

	else if (sw == schwarz )//schwarzabgleich
	{
		usb_send_str("\r\nSchwarzabgleich startet in 3sek \r\n");
		for (uint8_t i =0;i<8;i++)
		{
		red_black  += get_color_value(LED_RED);
		green_black+= get_color_value(LED_GREEN);
		blue_black += get_color_value(LED_BLUE);
		}
		red_black   /= 8;
		green_black /= 8;
		blue_black  /= 8;

		usb_send_str("\r\nSchwarzabgleich\r\nRED:\t");
		usb_send_int(red_black);
		usb_send_str("\r\nGREEN:\t");
		usb_send_int(green_black);
		usb_send_str("\r\nBLUE:\t");
		usb_send_int(blue_black);
			
			// diff	weiﬂ-schwarz
			//red_white,green_white,blue_white
		 red_diff= red_white-red_black;
		 green_diff=green_white-green_black;
		 blue_diff =blue_white-blue_black;
	}
}