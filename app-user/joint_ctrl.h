#ifndef __JOINT_CTRL_H__
#define __JOINT_CTRL_H__

#include "stdint.h"
#include "ros_rx.h"

typedef struct{
    float    ros_angle[5];
    float    current_angle[5];
    float    steering_angle[5];
    float    pwm[5];
    float    ctrl_cnt;
    
    point_t     point;
    
    uint8_t     finish;
}joint_t;

void joint_init(void);
void joint_control(void);
void joint_task(void const *argu);
void ros_angle_handler(float *ros_angle, float *steering_angle, int joint_num);
void angle2pwm(float *angle, float *pwm);



#endif
