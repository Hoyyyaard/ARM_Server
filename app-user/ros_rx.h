#ifndef __ROS_RX_H__
#define __ROS_RX_H__

#include "stdint.h"
#include "control_def.h"

typedef enum{
    HEAD1_ERROR = 0,
    HEAD2_ERROR = 1,
    TAIL_ERROR = 2,
    GOOD = 3
}ros_status_e;

__packed typedef struct{
    float angle[5];
    float flag; //��֡��־λ
}point_t;

typedef struct{
   /*����ͨ��״̬*/
    ros_status_e status;
    
    uint8_t success_rev_cnt;
    
    uint8_t rev_flag;
    
     __packed struct {           /* ���ݴ������� */    
        uint8_t head1;
        uint8_t head2; 
        
        point_t point[VIA_POINT_NUM];
        
        uint8_t eof;                        
    }rx_angle;
   
}ros_t;

void ros_single_data_handler(uint8_t *pData, uint8_t *rev_buf);
void ros_data_handler(uint8_t *pData);

#endif
