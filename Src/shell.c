/*
 * shell.c
 *
 *  Created on: Feb 11, 2018
 *      Author: homik
 */

#include <shell.h>
#include <stdlib.h>
#include <stdbool.h>

uint8_t rx_buffer = '\000';
uint8_t rx_string[SHELL_MAXSTRLEN];
int rx_index = 0;
int tx_busy = 0;

arguments_t args;

//list of shell commands


int shell_init(){

	args.number_of_arguments=0;

	//remeber to free these, you dingus;
	args.arguments_head = (arg_node_t*)malloc(sizeof(arg_node_t));

	args.arguments_head->is_head=true;
	args.arguments_head->is_tail=true;
	args.arguments_head->next_argument=args.arguments_tail;

	args.arguments_tail=args.arguments_head;
	args.current_argument=args.arguments_head;

	return 0;
}

void sprint(char string[]){
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
		sprint(" \b");
		if(rx_index > 0){
			rx_index--;
		}
		else rx_index=0;		//make sure it does not go negative
	}

	//check for enter and execute command

	else if (rx_buffer == '\n' || rx_buffer == '\r'){
		shell_execute(rx_string, rx_index);
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
	sprint("\r\n[ERROR] Serial error");
}

void shell_execute(uint8_t arg_string[], int arg_string_length){

	if(arg_string_length==0) return;   //don't bother executing if there are no arguments.

	//behold the Ouroboros

	//check to see if its the first argument entered. if so create the head.
	if(args.number_of_arguments==0){
		strcpy(args.arguments_head->arg_content, &arg_string);
		args.arguments_head->arg_length=arg_string_length;
		args.number_of_arguments=1;

		//execute the argument
		if(shell_tokenize_arguments(args.arguments_head)){
			sprint("[ERROR] Could not tokenize argument.");
		}
	}
	//if not see if we've stored the max number of arguments. eat the tail.
	else if(args.number_of_arguments>=SHELL_MAXARGS){
		//delete old tail
		arg_node_t * next_tail = args.arguments_tail->next_argument;
		next_tail->is_tail=true;
		next_tail->is_head=false;
		free(args.arguments_tail);

		//set new tail
		args.arguments_tail=next_tail;

		//create a new head
		arg_node_t * next_head = (arg_node_t*)malloc(sizeof(arg_node_t));
		next_head->is_head=true;
		next_head->is_tail=false;
		strcpy(next_head->arg_content, &arg_string);
		next_head->arg_length=arg_string_length;
		next_head->next_argument=args.arguments_tail;
		next_head->prev_argument=args.arguments_head;

		//set new head
		args.arguments_head->is_head=false;
		args.arguments_head=next_head;

		//execute the argument
		if(shell_tokenize_argument(args.arguments_head)){
			sprint("[ERROR] Could not tokenize argument.");
		}

	}
	//otherwise just create a new head.
	else{
		//create a new head
		arg_node_t * next_head = (arg_node_t*)malloc(sizeof(arg_node_t));
		next_head->is_head=true;
		next_head->is_tail=false;
		strcpy(next_head->arg_content, &arg_string);
		next_head->arg_length=arg_string_length;
		next_head->next_argument=args.arguments_tail;
		next_head->prev_argument=args.arguments_head;

		//set new head
		args.arguments_head->is_head=false;
		args.arguments_head=next_head;

		args.number_of_arguments++;

		//execute the argument
		if(shell_tokenize_argument(args.arguments_head)){
			sprint("[ERROR] Could not tokenize argument.");
		}
	}
}

int shell_tokenize_arguments(arg_node_t *argument){
	int arg_string_start = 0;
	int arg_amount=0;

	bool in_word=false;
	//count number of arguments. spaces delineate conent.
	for (int i=0; i <= (argument->arg_length); i++){
		if(!in_word && (argument->arg_content[i]!=' ')){
			in_word=true;
			arg_amount++;
		}
		else if(in_word && (argument->arg_content==' ')){
			in_word=false;
		}
	}

	//create a container for the arguments.

	//parse the arguments.
	for (int i = 0; i <= (argument->arg_length); i++){
		if((argument->arg_content)[i]== ' '){
			if(i==0) return 1;
		}
		else arg_string_start++;
	}

	return 0;
}

void string_partial_copy(char * dest[], char * src[], int start, int stop){
	for(int i=start; i <= (stop); i++){
		*dest[i] = *src[i];
	}
}

