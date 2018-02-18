/*
 * shell.c
 *
 *  Created on: Feb 11, 2018
 *      Author: homik
 */

#include <shell.h>

uint8_t rx_buffer = '\000';
uint8_t rx_string[SHELL_MAXSTRLEN];
int rx_index = 0;
int tx_busy = 0;

void echo(){
	print(rx_string);
}

void print(char string[]){
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 5);
}

void print_char(char c){
	HAL_UART_Transmit(&huart1, (uint8_t*)c, 1, 5);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	//clear buffer to prevent overrun
	__HAL_UART_FLUSH_DRREGISTER(&huart1);

	print_char(&rx_buffer);

	int i = 0;

	//check if backspace or del

	if (rx_buffer == 8 || rx_buffer == 127){
		print(" \b");
		if(rx_index > 0){
			rx_index--;
		}
		else rx_index=0;		//make sure it does not go negative
	}

	//check for enter and execute command

	else if (rx_buffer == '\n' || rx_buffer == '\r'){
		shell_execute(rx_string);
		rx_index = 0;
		for (i=0; i < SHELL_MAXSTRLEN; i++) rx_string[i]=0;
	}

	//otherwise add char to string

	else{
		if(rx_index <= SHELL_MAXSTRLEN){
			rx_string[rx_index] = rx_buffer;
			rx_index++;
		}
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	print("\r\n[ERROR] Serial error");
}

void shell_execute(uint8_t string[]){

}


