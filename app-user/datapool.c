#include "datapool.h"


joint_t joint;

BaseType_t xStatus_rx;

float test[5];

int time = 50;

int pub_flag[10];

BaseType_t xStatus_tx;

int tx_cnt;

ros_t ros;

//���ڽ��ջ���
uint8_t buf[ONE_BUF];
uint8_t max_buf[MAX_BUF_LEN];
uint8_t ros_buf[ROS_BUF_LEN];

ros_tx_t ros_tx;