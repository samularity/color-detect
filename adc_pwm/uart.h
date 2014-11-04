#ifndef _uart_included_h_
#define _uart_included_h_

#include <stdint.h>

void uart_init(uint32_t baud);
void uart_putchar(uint8_t c);
void uart_putstr(const char *str);
void uart_putint(uint32_t i,uint8_t base);
uint8_t uart_getchar(void);
uint8_t uart_available(void);

#endif
