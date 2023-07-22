#include "joint_ctrl.h"
/*��׼���ļ�*/


joint_t joint;
extern ros_t ros;
extern QueueHandle_t xViaPointQueue;

BaseType_t xStatus_rx;
int rev_cnt;
float test[5];

void joint_init(void)
{
    memset(&joint, 0, sizeof(joint_t));
    J0_PWM = Init_PWM;
    J1_PWM = Init_PWM;
    J2_PWM = Init_PWM;
    J3_PWM = Init_PWM;
    J4_PWM = Init_PWM;  
    for(int j=0;j<5;j++){
        joint.current_angle[j] = 90;
        joint.angle[j] = 90;
    }
}
//ROS�ࣺ�ӵ�������е�ۿ���J0˳ʱ������J1,J2,J3��ǰΪ����J4�н�Ϊ��
//��زࣺJ0��ʱ��Ϊ�������Ե���90��,J1����Ϊ����J2��ǰΪ����J3��ǰΪ����J4�н�Ϊ��
void joint_control(void)
{
        
//    if(!joint.point.flag){    //���ǲ�֡
//        for(int j=0;j<5;j++){                    
//            joint.ros_angle[j] = joint.point.angle[j];
//            
//            /* ros�Ƕ�ת��Ϊ������ԽǶ� */
//            ros_angle_handler(&joint.ros_angle[j], &joint.angle[j], j);
//            
//            if(!(joint.angle[j]<-90 | joint.angle[j]>90)){  
//                angle2pwm(&joint.angle[j], &joint.pwm[j]);
//                joint.current_angle[j] = joint.angle[j];
//            }
//        }
        
        J0_PWM = joint.pwm[0];
        J1_PWM = joint.pwm[1];
        J2_PWM = joint.pwm[2];
        J3_PWM = joint.pwm[3];
        J4_PWM = joint.pwm[4]; 
    //}   
        
           
}

void joint_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();
    joint_init();    
    for(;;) {
        taskENTER_CRITICAL();
          rev_cnt++;
        xStatus_rx = xQueueReceive( xViaPointQueue, &joint.point, 2);
        if( xStatus_rx == pdPASS ){
                    rev_cnt++;
                }
                  
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&thread_wake_time, 2); 
    }
}

/**
  * @brief ros��Ƕȴ���ת��Ϊ������ԽǶ�
  * @param ros_angle��ros��Ƕȣ�steering_angle������������ԽǶȣ�joint_num���ؽ���
  * @retval 
  */
void ros_angle_handler(float *ros_angle, float *steering_angle, int joint_num)
{
    switch(joint_num){
        case 0:
        case 1:{
            *steering_angle = 90 - *ros_angle;            
            break;}
        case 2:
        case 3:
        case 4:{
            *steering_angle = 90 + *ros_angle;
            break;}
    }
}

/**
  * @brief  ������ԽǶȿ���PWMת��
  * @param 
  * @retval 
  */
void angle2pwm(float *angle, float *pwm)
{
    *pwm = (*angle/9.0) + 5;
}


