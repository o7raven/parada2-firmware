#ifndef _SENSORS__H
#define _SENSORS__H

#include "drivers/bme_280/bme280.h"
#include "system/status.h"
#include "system/state_machine.h"
#include "drivers/hmc5883l/hmc5883l.h"
#include "drivers/IMU/imu.h"

typedef struct {
   bme280_t bme280; 
   hmc5883l_t hmc;
   imu_t imu;
} sensors_t;

status_t init_sensors(sensors_t* sens_handler, system_context_t* ctx);

status_t read_sensors(sensors_t* sens_handler, system_context_t* ctx);

status_t sensors_check_health(system_context_t* ctx);

#endif // _SENSORS__H