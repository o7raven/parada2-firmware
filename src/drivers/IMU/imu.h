#ifndef _IMU__H_
#define _IMU__H_

#include "system/status.h"
#include "inttypes.h"

#define mpu_slave_addr = 0x68;
#define mpu_chip_id = 0x70;

#define REG_WHO_AM_I = 0x75;
#define REG_PWR_MGMT = 0x6B;

#define G_2 0<<3
#define G_4 1<<3
#define G_8 2<<3
#define G_16 3<<3
#define AFS_SEL = G_4;
static const float ACC_LSB_SENSITIVITY = 8192.0f;

#define FSR_250 0 << 3
#define FSR_500 1 << 3
#define FSR_1000 2 << 3
#define FSR_2000 3 << 3
#define FS_SEL = FSR_500;
static const float GYRO_LSB_SENSITIVITY = 65.5f;
#define PLL_X_GYRO 0x01

#define REG_CONFIG = 0x1a;
#define mpu_dlpf_cfg = 0x03;


#define REG_ACCEL_CONFIG = 0x1c;
#define REG_GYRO_CONFIG = 0x1b;

#define REG_ACCEL_ADDR = 0x3B;
#define ACCEL_REG_LEN = 6;

#define REG_GYRO_ADDR = 0x43;
#define GYRO_REG_LEN = 6;


typedef struct{
    float acc_X;
    float acc_Y;
    float acc_Z;

    float gyro_X;
    float gyro_Y;
    float gyro_Z;
} imu_t;

status_t configure_imu(imu_t* imu_handler);
status_t read_imu(imu_t* imu_handler);
#endif // _IMU__H_