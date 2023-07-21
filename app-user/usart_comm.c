#include "usart_comm.h"

#include "usart.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "string.h"

#include "control_def.h"
#include "joint_ctrl.h"

//���ڽ��ջ���
uint8_t buf[ONE_BUF];

uint8_t rev_buf[DATA_BUF_LEN];

uint8_t ros_buf[ROS_BUF_LEN];

ros_t ros;
ros_tx_t ros_tx;
extern joint_t joint;

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

/*һ֡����һ��bufʱ�������ݰ�������HAL_UART_Receive_DMA(&huart1, buf, ONE_BUF); */
void ros_single_data_handler(uint8_t *pData)
{
    memcpy((rev_buf+ros.success_rev_cnt),pData, ONE_BUF); //��֡д�����ݻ�����
    ros.success_rev_cnt++;
    
    if(rev_buf[0] != 0xcc){  
        ros.status = HEAD1_ERROR;
        ros.success_rev_cnt=0;  
        memset(rev_buf, 0, DATA_BUF_LEN);  //��ջ����������½���
        return;
    }
    else if(ros.success_rev_cnt==2 && rev_buf[1]!=0xdc){ //���յ�֡ͷ2
        ros.status = HEAD2_ERROR;
        ros.success_rev_cnt=0;
        memset(rev_buf, 0, DATA_BUF_LEN);
        return;
    }
    
    if(ros.success_rev_cnt == ROS_BUF_LEN){     //��β�ж�
        if(rev_buf[ros.success_rev_cnt--] == 0xdd) {      
            memcpy(&ros.rx_angle, rev_buf, ROS_BUF_LEN);                
            memset(rev_buf, 0, DATA_BUF_LEN);  
        }
        else {      
            ros.status = TAIL_ERROR;
            ros.success_rev_cnt=0;
            memset(rev_buf, 0, DATA_BUF_LEN);
            return;
        }
    }
}

/*һ֡��������·����Ϣ���ݽ��*/
void ros_data_handler(uint8_t *pData)
{
    memcpy(&ros.rx_angle, pData, ROS_BUF_LEN);
    
    if(ros.rx_angle.head1 != 0Xcc){
        ros.status = HEAD1_ERROR;              
        return;
    }
    else if(ros.rx_angle.head2!=0XFF){
        ros.status = HEAD2_ERROR;              
        return;
    }
    else if(ros.rx_angle.eof != 0Xaa) {
        ros.status = TAIL_ERROR;              
        return;
    }
    else ros.status = GOOD;
        
}



void usart_msg_send_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();
    
    memset(&ros_tx, 0, ROS_TX_LEN);
    
    for(;;) {
        taskENTER_CRITICAL();
         /*֡ͷ֡β*/
        ros_tx.tx_msg.head1 = 0Xcc;
        ros_tx.tx_msg.head2 = 0Xff;
        ros_tx.tx_msg.eof = 0Xaa;
        
        ros_tx.tx_msg.finish_flag = joint.finish;
        
        memcpy(ros_tx.buff,&ros_tx.tx_msg, ROS_TX_LEN);
        
        HAL_UART_Transmit_DMA(&huart3,ros_tx.buff,ROS_TX_LEN);
                  
        taskEXIT_CRITICAL();
        osDelayUntil(&thread_wake_time, 1); 
    }
}
