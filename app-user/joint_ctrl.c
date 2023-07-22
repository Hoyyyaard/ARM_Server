#include "joint_ctrl.h"
/*标准库文件*/
#include "control_def.h"
#include "math.h"
#include "string.h"
#include "cmsis_os.h"
#include "tim.h"

#include "usart_comm.h"

joint_t joint;
extern ros_t ros;
extern QueueHandle_t xViaPointQueue;

BaseType_t xStatus_rx;
float test[5];

int time = 50;

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
        joint.steering_angle[j] = 90;
        joint.pwm[j] = 15;
    }
}

//ROS侧：从底盘往机械臂看，J0顺时针正，J1,J2,J3朝前为正，J4夹紧为正
//电控侧：J0逆时针为正，背对底盘90度,J1朝后为正，J2朝前为正，J3朝前为正，J4夹紧为正
void joint_control(void)
{    
    if(joint.point.flag){    //不是补帧
        for(int j=0;j<5;j++){                    
            joint.ros_angle[j] = joint.point.angle[j];
            
            /* ros角度转换为舵机绝对角度 */
            ros_angle_handler(&joint.ros_angle[j], &joint.steering_angle[j], j);
            
            if(!(joint.steering_angle[j]<10 | joint.steering_angle[j]>165)){  
                angle2pwm(&joint.steering_angle[j], &joint.pwm[j]);
                joint.current_angle[j] = joint.steering_angle[j];
            }
        }       
        J0_PWM = joint.pwm[0];
        J1_PWM = joint.pwm[1];
        J2_PWM = joint.pwm[2];
        J3_PWM = joint.pwm[3];
        J4_PWM = joint.pwm[4]; 
    }             
}

void joint_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();
    joint_init();    
    for(;;) {
        taskENTER_CRITICAL();
        
        xStatus_rx = xQueueReceive(xViaPointQueue, &joint.point, 0);       
        if( xStatus_rx == pdPASS ){
            joint.finish = 0;
            joint_control();
        }
        else joint.finish = 1; //队列为空，一帧路点处理完成，进入等待
                  
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&thread_wake_time, time);
    }
}

/**
  * @brief ros侧角度处理，转换为舵机绝对角度
  * @param ros_angle：ros侧角度，steering_angle：处理后舵机绝对角度，joint_num：关节数
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
  * @brief  舵机绝对角度控制PWM转换
  * @param 
  * @retval 
  */
void angle2pwm(float *angle, float *pwm)
{
    *pwm = (*angle/9.0) + 5;
}


