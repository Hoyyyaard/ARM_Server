#ifndef __DATAPOOL_H__
#define __DATAPOLL_H__

/* include */
#include "usart.h"
#include "tim.h"
#include "stdint.h"
#include "ros_rx.h"
#include "control_def.h"

/******************************* ROS������Ϣ *********************************/
#define VIA_POINT_NUM 10
#define ONE_BUF 1
#define MAX_BUF_LEN 1000  
#define ROS_BUF_LEN (VIA_POINT_NUM*24)+3
#define ROS_TX_LEN 5

/*****************************steering control* ****************************/
#define Init_PWM    15

#define J0_PWM        TIM2->CCR2 //PB3
#define J1_PWM        TIM4->CCR3    //PB8
#define J2_PWM        TIM4->CCR4   //PB9
#define J3_PWM        TIM4->CCR1  //PB6
#define J4_PWM        TIM4->CCR2   //PB7

/*��ز෢�����ݰ�*/
typedef struct{
    uint8_t buff[ROS_TX_LEN];    
    __packed struct{           
        uint8_t head1;      
        uint8_t head2;      
        uint8_t rev_flag;
        uint8_t finish_flag;
        uint8_t eof;
    }tx_msg;       
    uint8_t tx_flag;
}ros_tx_t;

typedef struct{
    float    ros_angle[5];
    float    current_angle[5];
    float    steering_angle[5];
    float    pwm[5];
    float    ctrl_cnt;
    point_t     point;
    uint8_t     finish;
}joint_t;

typedef enum{
    REVING = 0,
    HEAD1_ERROR = 1,
    HEAD2_ERROR = 2,
    TAIL_ERROR = 3,
    GOOD = 4
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
    uint8_t good_flag;
    uint8_t last_rev_flag;
     __packed struct {           /* ���ݴ��������� */    
        uint8_t head1;
        uint8_t head2; 
        point_t point[VIA_POINT_NUM];
        uint8_t eof;                        
    }rx_angle;
}ros_t;


/* value */
extern joint_t joint;

extern BaseType_t xStatus_rx;

extern float test[5];

extern int time = 50;

extern int pub_flag[10];

extern BaseType_t xStatus_tx;

extern int tx_cnt;

extern ros_t ros;

//���ڽ��ջ���
extern uint8_t buf[ONE_BUF];
extern uint8_t max_buf[MAX_BUF_LEN];
extern uint8_t ros_buf[ROS_BUF_LEN];

extern ros_tx_t ros_tx;

#endif
