/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "gpio.h"
#include "stm32f7xx_hal.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

void MX_USART1_UART_Init(void)
{

	//init uart

  	huart1.Instance = USART1;
	huart1.Init.BaudRate = BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_7B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
    /* clocks enable */
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA10     ------> USART1_RX
    PA9     ------> USART1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //init DMA

	hdma_usart1_rx.Instance = DMA2_Stream2;
	hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_DISABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&hdma_usart1_rx);

	hdma_usart1_tx.Instance = DMA2_Stream7;
	hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_tx.Init.MemInc = DMA_MINC_DISABLE;
	hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_tx.Init.Mode = DMA_NORMAL;
	hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_usart1_tx.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_usart1_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	HAL_DMA_Init(&hdma_usart1_tx);

	__HAL_LINKDMA(huart, hdmarx, hdma_usart1_rx);
	__HAL_LINKDMA(huart, hdmatx, hdma_usart1_tx);

	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, UART_PRIORITY, UART_RX_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, UART_PRIORITY, UART_TX_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA10     ------> USART1_RX
    PA9     ------> USART1_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10|GPIO_PIN_9);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

    HAL_DMA_DeInit(huart->hdmarx);
    HAL_DMA_DeInit(huart->hdmatx);

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
