#ifndef __USART_COMM_H__
#define __USART_COMM_H__

#include "stdint.h"
#include "usart.h"

#include "control_def.h"

typedef enum{
    HEAD1_ERROR = 0,
    HEAD2_ERROR = 1,
    TAIL_ERROR = 2,
    GOOD = 3
}ros_status_e;

__packed typedef struct{
    float angle[5];
    float flag; //补帧标志位
}point_t;

typedef struct{
   /*串口通信状态*/
    ros_status_e status;
    
    uint8_t success_rev_cnt;
    
     __packed struct {           /* 数据处理共用体 */    
        uint8_t head1;
        uint8_t head2; 
        
        point_t point[VIA_POINT_NUM];
        
        uint8_t eof;                        
    }rx_angle;
   
}ros_t;

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
   


void USER_UART_IDLECallback(UART_HandleTypeDef *huart);
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void USER_USART_Init(void);

void ros_single_data_handler(uint8_t *pData);
void ros_data_handler(uint8_t *pData);

void usart_msg_send_task(void const *argu);

extern ros_t ros;

#endif
