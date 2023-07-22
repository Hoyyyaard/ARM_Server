#ifndef __USART_SEND_MSG_H__
#define __USART_SEND_MSG_H__

#include "stdint.h"

#include "control_def.h"

/*电控侧发送数据包*/
typedef struct{
    uint8_t buff[ROS_TX_LEN];    
    __packed struct{           
        uint8_t head1;      
        uint8_t head2;      

        uint8_t finish_flag;

        uint8_t eof;
    }tx_msg;                  
}ros_tx_t;

void usart_msg_send_task(void const *argu);

#endif
