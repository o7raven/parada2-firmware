#ifndef _IMU__H_
#define _IMU__H_

#include "system/status.h"
#include "inttypes.h"

static uint8_t mpu_slave_addr = 0x68;
static uint8_t mpu_chip_id = 0x68;

static uint8_t REG_WHO_AM_I = 0x75;
static uint8_t REG_PWR_MGMT = 0x6B;

#define G_2 0<<3
#define G_4 1<<3
#define G_8 2<<3
#define G_16 3<<3
static uint8_t AFS_SEL = G_4;
static const float ACC_LSB_SENSITIVITY = 8192.0f;

#define FSR_250 0 << 3
#define FSR_500 1 << 3
#define FSR_1000 2 << 3
#define FSR_2000 3 << 3
static uint8_t FS_SEL = FSR_500;

static const float GYRO_LSB_SENSITIVITY = 65.5f;
static uint8_t PLL_X_GYRO = 0x01;

static uint8_t REG_CONFIG = 0x1a;
static uint8_t mpu_dlpf_cfg = 0x03;


static uint8_t REG_ACCEL_CONFIG = 0x1c;
static uint8_t REG_GYRO_CONFIG = 0x1b;

static uint8_t REG_ACCEL_ADDR = 0x3B;
static uint8_t ACCEL_REG_LEN = 6;

static uint8_t REG_GYRO_ADDR = 0x43;
static uint8_t GYRO_REG_LEN = 6;


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