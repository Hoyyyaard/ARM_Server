#ifndef __CONTROL_DEF_H__
#define __CONTROL_DEF_H__

#include "usart.h"
#include "tim.h"
/******************************* ROS½»»¥ÐÅÏ¢ *********************************/
#define VIA_POINT_NUM 10

#define ONE_BUF 1
#define DATA_BUF_LEN 1000  

#define ROS_BUF_LEN (VIA_POINT_NUM*24)+3
#define ROS_TX_LEN 4

/*****************************steering control* ****************************/
#define JOINT_PERIOD 2

#define Init_PWM    15

#define J0_PWM        TIM2->CCR2 //PB3

#define J1_PWM        TIM4->CCR3    //PB8
#define J2_PWM        TIM4->CCR4   //PB9
#define J3_PWM        TIM4->CCR1  //PB6
#define J4_PWM        TIM4->CCR2   //PB7
#endif

