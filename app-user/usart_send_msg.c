#include "usart_send_msg.h"

#include "usart.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "string.h"

#include "control_def.h"
#include "joint_ctrl.h"

ros_tx_t ros_tx;
extern joint_t joint;

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
