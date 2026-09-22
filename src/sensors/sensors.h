#ifndef _SENSORS__H
#define _SENSORS__H

#include "drivers/IMU/imu.h"
#include "drivers/bme_280/bme280_driver.h"
#include "system/status.h"
#include "drivers/hmc5883l/hmc5883l.h"

typedef struct {
   struct bme280_data bme280; 
   hmc5883l_t hmc;
   imu_t imu;
} sensors_t;

typedef struct{
   bool bme280_ok;
   bool hmc5883l_ok;
   bool imu_ok;

   bool sensors_ok;
} sensors_context_t;

status_t init_sensors(sensors_t* sens_handler, sensors_context_t* ctx);

status_t read_sensors(sensors_t* sens_handler, sensors_context_t* ctx);

status_t sensors_check_health(sensors_context_t* ctx);

#endif // _SENSORS__H