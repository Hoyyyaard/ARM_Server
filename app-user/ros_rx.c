#include "ros_rx.h"
#include "string.h"
#include "control_def.h"

ros_t ros;

/**
  * @brief 一帧接收一个buf时解析数据包
  * @param *pData：串口接收一个buf，*rev_buf：设置最大缓冲区
  * @retval 
  */
void ros_single_data_handler(uint8_t *pData, uint8_t *rev_buf)
{
    memcpy((rev_buf+ros.success_rev_cnt),pData, ONE_BUF); //逐帧写入数据缓冲区
    ros.success_rev_cnt++;
    
    if(rev_buf[0] != 0xcc){  
        ros.status = HEAD1_ERROR;
        ros.success_rev_cnt=0;  
        memset(rev_buf, 0, MAX_BUF_LEN);  //清空缓冲区，重新接收
        return;
    }
    else if(ros.success_rev_cnt==2 && rev_buf[1]!=0xdc){ //接收到帧头2
        ros.status = HEAD2_ERROR;
        ros.success_rev_cnt=0;
        memset(rev_buf, 0, MAX_BUF_LEN);
        return;
    }
    
    if(ros.success_rev_cnt == ROS_BUF_LEN){     //包尾判断
        if(rev_buf[ros.success_rev_cnt--] == 0xdd) {      
            memcpy(&ros.rx_angle, rev_buf, ROS_BUF_LEN);                
            memset(rev_buf, 0, MAX_BUF_LEN);  
        }
        else {      
            ros.status = TAIL_ERROR;
            ros.success_rev_cnt=0;
            memset(rev_buf, 0, MAX_BUF_LEN);
            return;
        }
    }
}

/*一帧接收所有路点信息数据解包*/
void ros_data_handler(uint8_t *pData)
{
    memset(&ros, 0, sizeof(ros_t));
    memcpy(&ros.rx_angle, pData, ROS_BUF_LEN);
    
    ros.last_rev_flag = ros.rev_flag; //更新历史接收状态
    if(ros.rx_angle.head1 != 0Xcc){
        ros.status = HEAD1_ERROR;    
        ros.rev_flag = 0;
        return;
    }
    else if(ros.rx_angle.head2!=0XFF){
        ros.status = HEAD2_ERROR;    
        ros.rev_flag = 0;
        return;
    }
    else if(ros.rx_angle.eof != 0Xaa) {
        ros.status = TAIL_ERROR;  
        ros.rev_flag = 0;
        return;
    }
    else {
        ros.status = GOOD;
        ros.rev_flag = 1;
        ros.good_flag = 1;
    }
}

