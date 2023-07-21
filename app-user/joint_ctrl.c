#include "joint_ctrl.h"
/*标准库文件*/
#include "string.h"
#include "stdint.h"
#include "cmsis_os.h"
#include "tim.h"
/*用户头文件*/
#include "control_def.h"
#include "usart_comm.h"

joint_t joint;
extern ros_t ros;

void joint_init(void)
{
    memset(&joint, 0, sizeof(joint_t));
    J0_PWM = Init_PWM;
    J1_PWM = Init_PWM;
    J2_PWM = Init_PWM;
    J3_PWM = Init_PWM;
    J4_PWM = Init_PWM;    
}

void joint_control(void)
{
    if(ros.status  == GOOD){ 
        for(int i=0;i<VIA_POINT_NUM;i++){
            if(!ros.rx_angle.point[i].flag){    //不是补帧
                for(int j=0;j<5;j++){
                joint.targer_angle[j] = ros.rx_angle.point[i].angle[j];
                }
                /* 目标角度转换为PWM */
                
                //ROS侧：从底盘往机械臂看，J0顺时针正，J1,J2,J3朝前为正，J4夹紧为正
                //电控侧：J0逆时针为正，背对底盘90度
                
                J0_PWM = joint.pwm[0];
                J1_PWM = joint.pwm[1];
                J2_PWM = joint.pwm[2];
                J3_PWM = joint.pwm[3];
                J4_PWM = joint.pwm[4]; 
            }
        }
    
    }
}

void joint_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();
    joint_init();
    for(;;) {
        taskENTER_CRITICAL();
        
        joint_control();
                  
        taskEXIT_CRITICAL();
        osDelayUntil(&thread_wake_time, 2); 
    }
}

