#ifndef _IMU__H_
#define _IMU__H_

#include "system/status.h"
typedef struct{

} imu_t;

status_t configure_imu(imu_t* imu_handler);
status_t read_imu(imu_t* imu_handler);
#endif // _IMU__H_