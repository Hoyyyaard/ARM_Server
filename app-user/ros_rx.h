#ifndef __ROS_RX_H__
#define __ROS_RX_H__

#include "datapool.h"

void ros_single_data_handler(uint8_t *pData, uint8_t *rev_buf);
void ros_data_handler(uint8_t *pData);

#endif
