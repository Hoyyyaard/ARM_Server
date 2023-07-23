#ifndef __JOINT_CTRL_H__
#define __JOINT_CTRL_H__

#include "datapool.h"

void joint_init(void);
void joint_control(void);
void joint_task(void const *argu);
void ros_angle_handler(float *ros_angle, float *steering_angle, int joint_num);
void angle2pwm(float *angle, float *pwm);



#endif
