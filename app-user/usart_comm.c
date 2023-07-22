#include "usart_comm.h"

#include "usart.h"
#include "string.h"

#include "control_def.h"
#include "ros_rx.h"

//串口接收缓冲
uint8_t buf[ONE_BUF];
uint8_t max_buf[MAX_BUF_LEN];

uint8_t ros_buf[ROS_BUF_LEN];

void USER_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) 
    {
        ros_data_handler(ros_buf);
        memset(ros_buf, 0, ROS_BUF_LEN);
        HAL_UART_Receive_DMA(huart, ros_buf, ROS_BUF_LEN);
    } 
    else if (huart->Instance == USART3)  
    {
        ros_data_handler(ros_buf);
        memset(ros_buf, 0, ROS_BUF_LEN);
        HAL_UART_Receive_DMA(huart, ros_buf, ROS_BUF_LEN);
    }
}

/*串口空闲中断,在.it文件中调用*/
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{   
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) //判断是否是空闲中断
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);                     //清除空闲中断标志（否则会一直不断进入中断）
        HAL_UART_DMAStop(huart);							//停止本次DMA运输
        USER_UART_IDLECallback(huart);                     //调用串口功能回调函数
    }
}

void USER_USART_Init(void)
{
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);  //清除空闲中断标志
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //使能串口中断
    HAL_UART_Receive_DMA(&huart1, ros_buf, ROS_BUF_LEN); //开始一次接收
    
     __HAL_UART_CLEAR_IDLEFLAG(&huart3);  //清除空闲中断标志
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);  //使能串口中断
    HAL_UART_Receive_DMA(&huart3, ros_buf, ROS_BUF_LEN); //开始一次接收
    
}
