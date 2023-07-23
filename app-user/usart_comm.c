#include "usart_comm.h"

#include "usart.h"
#include "string.h"

#include "control_def.h"
#include "ros_rx.h"

extern ros_t ros;


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

/*���ڿ����ж�,��.it�ļ��е���*/
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{   
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) //�ж��Ƿ��ǿ����ж�
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);                     //��������жϱ�־�������һֱ���Ͻ����жϣ�
        HAL_UART_DMAStop(huart);							//ֹͣ����DMA����
        USER_UART_IDLECallback(huart);                     //���ô��ڹ��ܻص�����
    }
}

void USER_USART_Init(void)
{
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);  //��������жϱ�־
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //ʹ�ܴ����ж�
    HAL_UART_Receive_DMA(&huart1, ros_buf, ROS_BUF_LEN); //��ʼһ�ν���
    
     __HAL_UART_CLEAR_IDLEFLAG(&huart3);  //��������жϱ�־
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);  //ʹ�ܴ����ж�
    HAL_UART_Receive_DMA(&huart3, ros_buf, ROS_BUF_LEN); //��ʼһ�ν���
    
}
