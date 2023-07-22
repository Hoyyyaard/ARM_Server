#include "ros_msg_pub.h"

#include "cmsis_os.h"

#include "control_def.h"
#include "ros_rx.h"
#include "joint_ctrl.h"

extern ros_t ros;
extern joint_t joint;
extern QueueHandle_t xViaPointQueue;

int pub_flag[10];
BaseType_t xStatus_tx;
int tx_cnt;

void ros_msg_pub_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();   
    for(;;) {
        taskENTER_CRITICAL();
        
        if(joint.finish == 1 && ros.good_flag == 1){
            ros.good_flag = 0;
            for(int i=0; i<VIA_POINT_NUM; i++){ 
                xStatus_tx = xQueueSend( xViaPointQueue, &ros.rx_angle.point[i], 0);
                if( xStatus_tx == pdPASS ){
                    pub_flag[i] = 1;
                } 
            } 
            
        }
            
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&thread_wake_time, 2); 
    }
}

