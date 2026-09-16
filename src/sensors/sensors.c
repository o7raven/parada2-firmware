#include "sensors.h"
#include "communication/logging.h"
#include "drivers/IMU/imu.h"
#include "drivers/bme_280/bme280_driver.h"
#include "drivers/hmc5883l/hmc5883l.h"
#include "system/state_machine.h"
#include "system/status.h"
#include "config/system_config.h"

status_t init_sensors(sensors_t* sens_handler, system_context_t* ctx){
    ctx->sensors_ok = true;

    status_t init_status = STATUS_OK;
    if(configure_bme280() != STATUS_OK){
        log_error("Config error", STATUS_SENSOR_ERROR_BME_CONFIG);
        init_status = STATUS_SENSOR_INIT_FAIL;

        ctx->sensors_ok = false;
    }

    if(configure_hmc(&(sens_handler->hmc))!=STATUS_OK){
        log_error("Config error",STATUS_SENSOR_ERROR_HMC_CONFIG);
        init_status = STATUS_SENSOR_INIT_FAIL;

        ctx->sensors_ok = false;
    }

    if(configure_imu(&(sens_handler->imu))!=STATUS_OK){
        log_error("Config error",STATUS_SENSOR_ERROR_IMU_CONFIG);
        init_status = STATUS_SENSOR_INIT_FAIL;

        ctx->sensors_ok = false;
    }
    return init_status;
}

status_t read_sensors(sensors_t* sens_handler, system_context_t* ctx){
    log_info("Reaing BME280 ...");

    if(sens_handler == NULL){
        ctx->sensors_ok = false;
        log_error("Sens handler is null", STATUS_I2C_ERROR);
    }

    if(bme280_scan(&sens_handler->bme280) != STATUS_OK){
        ctx->sensors_ok = false;
        log_warning("BME280 Reading faiure...");
    }else {
        log_success("BME280 Reading successfull");
        log_sensor("BME280\t(temp,press,hum):\t%fC\t%f ...\t%f ...",
                   sens_handler->bme280.temperature,
                   sens_handler->bme280.pressure,
                   sens_handler->bme280.humidity);
    }

    if(read_hmc(&sens_handler->hmc) != STATUS_OK){
        ctx->sensors_ok = false;
        log_error("Read error",STATUS_SENSOR_ERROR_HMC_READ);
    }
    if(read_imu(&sens_handler->imu) != STATUS_OK){
        ctx->sensors_ok = false;
        log_error("Read error",STATUS_SENSOR_ERROR_IMU_READ);

    }


    return STATUS_OK;
}

status_t sensors_check_health(system_context_t* ctx){
    return STATUS_NOT_IMPLEMENTED;
}