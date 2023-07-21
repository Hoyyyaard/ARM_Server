#include "joint_ctrl.h"
/*��׼���ļ�*/
#include "string.h"
#include "stdint.h"
#include "cmsis_os.h"
#include "tim.h"
/*�û�ͷ�ļ�*/
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
            if(!ros.rx_angle.point[i].flag){    //���ǲ�֡
                for(int j=0;j<5;j++){
                joint.targer_angle[j] = ros.rx_angle.point[i].angle[j];
                }
                /* Ŀ��Ƕ�ת��ΪPWM */
                
                //ROS�ࣺ�ӵ�������е�ۿ���J0˳ʱ������J1,J2,J3��ǰΪ����J4�н�Ϊ��
                //��زࣺJ0��ʱ��Ϊ�������Ե���90��
                
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

