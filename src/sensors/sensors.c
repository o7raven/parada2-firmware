#include "sensors.h"
#include "drivers/IMU/imu.h"
#include "drivers/bme_280/bme280.h"
#include "drivers/hmc5883l/hmc5883l.h"
#include "system/state_machine.h"
#include "system/status.h"

status_t init_sensors(sensors_t* sens_handler, system_context_t* ctx){
    ctx->sensors_ok = false;

    if(configure_bme280(&(sens_handler->bme280)) != STATUS_OK){
        return STATUS_SENSOR_ERROR_BME_CONFIG;
    }
    if(configure_hmc(&(sens_handler->hmc))!=STATUS_OK){
        return STATUS_SENSOR_ERROR_HMC_CONFIG;
    }
    if(configure_imu(&(sens_handler->imu))!=STATUS_OK){
        return STATUS_SENSOR_ERROR_IMU_CONFIG;
    }
    return STATUS_OK;
}

status_t read_sensors(sensors_t* sens_handler, system_context_t* ctx){
    if(read_bme(&sens_handler->bme280) != STATUS_OK){
        return STATUS_SENSOR_ERROR_BME_READ;
    }
    if(read_hmc(&sens_handler->hmc) != STATUS_OK){
        return STATUS_SENSOR_ERROR_HMC_READ;
    }
    if(read_imu(&sens_handler->imu) != STATUS_OK){
        return STATUS_SENSOR_ERROR_IMU_READ;
    }
    return STATUS_OK;
}

status_t sensors_check_health(system_context_t* ctx){
    return STATUS_NOT_IMPLEMENTED;
}