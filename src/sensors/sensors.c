#include "sensors.h"
#include "communication/logging.h"
#include "drivers/IMU/imu.h"
#include "drivers/bme_280/bme280_driver.h"
#include "drivers/hmc5883l/hmc5883l.h"
#include "system/state_machine.h"
#include "system/status.h"
#include "config/system_config.h"

status_t init_sensors(sensors_t* sens_handler, system_context_t* ctx){
    ctx->sensors_ok = false;

    if(configure_bme280() != STATUS_OK){
        log_error("Config error", STATUS_SENSOR_ERROR_BME_CONFIG);
    }

    if(configure_hmc(&(sens_handler->hmc))!=STATUS_OK){
        log_error("Config error",STATUS_SENSOR_ERROR_HMC_CONFIG);
    }

    if(configure_imu(&(sens_handler->imu))!=STATUS_OK){
        log_error("Config error",STATUS_SENSOR_ERROR_IMU_CONFIG);
    }
    return STATUS_OK;
}

status_t read_sensors(sensors_t* sens_handler, system_context_t* ctx){
    log_info("Reading bme");
    if(bme280_scan(&sens_handler->bme280) != STATUS_OK){
        log_error("Read error", STATUS_SENSOR_ERROR_BME_READ);
    }
    if(sens_handler == NULL){
        log_error("Sens handler is null", STATUS_I2C_ERROR);
    }
    log_info("Bme: %f", sens_handler->bme280.temperature);
    log_info("pressure: %f", sens_handler->bme280.pressure);
    log_info("humidity: %f", sens_handler->bme280.humidity);

    /*if(read_hmc(&sens_handler->hmc) != STATUS_OK){
        log_error("Read error",STATUS_SENSOR_ERROR_HMC_READ);
    }
    if(read_imu(&sens_handler->imu) != STATUS_OK){
        log_error("Read error",STATUS_SENSOR_ERROR_IMU_READ);

    }*/
    return STATUS_OK;
}

status_t sensors_check_health(system_context_t* ctx){
    return STATUS_NOT_IMPLEMENTED;
}