/*
 * shell.h
 *
 *  Created on: Feb 11, 2018
 *      Author: homik
 */

#ifndef SHELL_H_
#define SHELL_H_


#include "usart.h"
#include "gpio.h"
#include "string.h"

#define SHELL_MAXSTRLEN 100

struct argument{
	char arg_name[];
	struct argument *next_argument;
};

void echo();
void print(char string[]);
void print_char(char c);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
void shell_execute(uint8_t string[], int length);
struct argument * shell_parse_command(uint8_t *string[], int *length);

#endif /* SHELL_H_ */
