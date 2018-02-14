/*
 * vTerminal.h
 *
 *  Created on: Feb 11, 2018
 *      Author: homik
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_


#include "usart.h"
#include "gpio.h"

void terminal_putc(char* c);
char terminal_getc(void);
void echo();
void vTerminal(void);

#endif /* TERMINAL_H_ */
