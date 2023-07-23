#ifndef __USART_COMM_H__
#define __USART_COMM_H__

#include "usart.h"
#include "datapool.h"

void USER_UART_IDLECallback(UART_HandleTypeDef *huart);
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void USER_USART_Init(void);

#endif
