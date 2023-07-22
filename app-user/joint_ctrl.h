#ifndef __JOINT_CTRL_H__
#define __JOINT_CTRL_H__

#include "stdint.h"
#include "control_def.h"
#include "usart_comm.h"
#include "string.h"
#include "cmsis_os.h"
#include "tim.h"


typedef struct{
    float    ros_angle[5];
    float    current_angle[5];
    float    angle[5];
    float    pwm[5];
    
    point_t     point;
    uint8_t     finish;
    float    ctrl_cnt;

}joint_t;

void joint_init(void);
void joint_control(void);
void joint_task(void const *argu);
void ros_angle_handler(float *ros_angle, float *steering_angle, int joint_num);
void angle2pwm(float *angle, float *pwm);

extern joint_t joint;

#endif
