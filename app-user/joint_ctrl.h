#ifndef __JOINT_CTRL_H__
#define __JOINT_CTRL_H__

#include "stdint.h"

typedef struct{
    uint16_t    targer_angle[5];
    uint16_t    pwm[5];
    
    uint8_t     finish;

}joint_t;

void joint_init(void);
void joint_control(void);
void joint_task(void const *argu);

extern joint_t joint;

#endif
