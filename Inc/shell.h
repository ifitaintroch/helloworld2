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

struct Command{

} command;

void echo();
void print(char string[]);
void print_char(char c);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
void shell_execute(uint8_t command());
void vShell(void);

#endif /* SHELL_H_ */
