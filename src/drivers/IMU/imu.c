#include "imu.h"
#include "communication/logging.h"
#include "drivers/i2c/i2c.h"
#include "system/status.h"

status_t configure_imu(imu_t* imu_handler){
    uint8_t _chip_Id_buffer[1];
    abstract_i2c_read(REG_WHO_AM_I, _chip_Id_buffer, 1, mpu_slave_addr);
    if(*_chip_Id_buffer!=mpu_chip_id){
        log_warning("The IMU has not been found!");
        return STATUS_NOT_FOUND;
    }
    log_success("IMU has been found"); 
    uint8_t config_set = 0x00;
    abstract_i2c_write(REG_PWR_MGMT, &config_set, 1, mpu_slave_addr);
    abstract_i2c_write(REG_PWR_MGMT, &PLL_X_GYRO, 1, mpu_slave_addr);

    abstract_i2c_write(REG_ACCEL_CONFIG, &AFS_SEL, 1, mpu_slave_addr);
    abstract_i2c_write(REG_GYRO_CONFIG, &FS_SEL, 1, mpu_slave_addr);
    abstract_i2c_write(REG_CONFIG, &mpu_dlpf_cfg, 1, mpu_slave_addr);

    return STATUS_OK;
}
status_t read_imu(imu_t* imu_handler){
  int16_t i16_accX;
  int16_t i16_accY;
  int16_t i16_accZ;

  int16_t i16_gyroX;
  int16_t i16_gyroY;
  int16_t i16_gyroZ;

  uint8_t acc_data[ACCEL_REG_LEN];
  abstract_i2c_read(REG_ACCEL_ADDR, acc_data, ACCEL_REG_LEN, mpu_slave_addr);
  uint8_t gyro_data[GYRO_REG_LEN];
  abstract_i2c_read(REG_GYRO_ADDR, gyro_data, ACCEL_REG_LEN, mpu_slave_addr);

  i16_accX = (acc_data[0] << 8) | acc_data[1];
  i16_accY = (acc_data[2] << 8) | acc_data[3];
  i16_accZ = (acc_data[4] << 8) | acc_data[5];

  imu_handler->acc_X=i16_accX/ACC_LSB_SENSITIVITY;
  imu_handler->acc_Y=i16_accY/ACC_LSB_SENSITIVITY;
  imu_handler->acc_Z=i16_accZ/ACC_LSB_SENSITIVITY;

  i16_gyroX = (gyro_data[0] << 8) | gyro_data[1];
  i16_gyroY = (gyro_data[2] << 8) | gyro_data[3];
  i16_gyroZ = (gyro_data[4] << 8) | gyro_data[5];

  imu_handler->gyro_X = i16_gyroX / GYRO_LSB_SENSITIVITY;
  imu_handler->gyro_Y = i16_gyroY / GYRO_LSB_SENSITIVITY;
  imu_handler->gyro_Z = i16_gyroZ / GYRO_LSB_SENSITIVITY;

  return STATUS_OK;
}