/*
 * shell.h
 *
 *  Created on: Feb 11, 2018
 *      Author: homik
 */

#include <stdbool.h>

#ifndef SHELL_H_
#define SHELL_H_


#include "usart.h"
#include "gpio.h"
#include "string.h"

#define SHELL_MAXSTRLEN 100
#define SHELL_MAXARGS 20

typedef struct arg_node{
	struct arg_node * prev_argument;
	struct arg_node * next_argument;
	bool is_head;
	bool is_tail;
	int arg_length;
	char arg_content[SHELL_MAXSTRLEN];

} arg_node_t;

typedef struct arguments{
	arg_node_t * arguments_head;
	arg_node_t * arguments_tail;
	arg_node_t * current_argument;
	int number_of_arguments;
}arguments_t;

//main shell functions

int shell_init();
void sSprint(char string[]);
void print_char(char c);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
void shell_execute(uint8_t string[], int length);
int shell_tokenize_arguments(arg_node_t *arguments);

//shell commands

#endif /* SHELL_H_ */
