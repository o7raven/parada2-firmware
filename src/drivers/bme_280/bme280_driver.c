#include "bme280_driver.h"
#include "communication/logging.h"
#include "drivers/i2c/i2c.h"
#include "libs/bosch/bme280.h"
#include "libs/bosch/bme280_defs.h"
#include "system/status.h"
#include <pico/time.h>
#include <stdint.h>

static struct bme280_dev dev;
static uint32_t period;
static uint8_t dev_addr = BME280_I2C_ADDR_PRIM;



// @Note add proper error handling
status_t configure_bme280(){
    uint8_t rslt;

    struct bme280_settings settings;
    log_info("Configuring bme280 ...");
    

    dev.read = bme280_i2c_read;
    dev.write = bme280_i2c_write;

    dev.intf = BME280_I2C_INTF;
    dev.intf_ptr = &dev_addr;

    dev.delay_us = bme280_delay_us;

    //sleep before config
    rslt = bme280_init(&dev);
    if(rslt!= BME280_OK){
        log_error("BME280 initialization failed", STATUS_SENSOR_ERROR_BME_CONFIG);
        return STATUS_SENSOR_ERROR_BME_CONFIG;
    }
    sleep_ms(100);



    rslt = bme280_get_sensor_settings(&settings, &dev);
    if(rslt!= BME280_OK){
        log_error("Failed to GET BME280 settings", STATUS_SENSOR_ERROR_BME_CONFIG);
    }
    settings.filter = BME280_FILTER_COEFF_2;
    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_1X;
    settings.osr_t = BME280_OVERSAMPLING_1X;

    settings.standby_time = BME280_STANDBY_TIME_0_5_MS;

    rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &dev);
    if(rslt!= BME280_OK){
        log_error("Failed to SET BME280 settings", STATUS_SENSOR_ERROR_BME_CONFIG);
        return STATUS_SENSOR_ERROR_BME_CONFIG;
    }

    rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);

    if(rslt != BME280_OK){
        log_error("Failed to SET BME280 sensor mode", STATUS_SENSOR_ERROR_BME_CONFIG);
        return STATUS_SENSOR_ERROR_BME_CONFIG;
    }

    rslt = bme280_cal_meas_delay(&period, &settings);

    if(rslt != BME280_OK){
        log_error("Failed to calculate measurement delay", STATUS_SENSOR_ERROR_BME_CONFIG);
        return STATUS_SENSOR_ERROR_BME_CONFIG;
    }

    log_info("Measurement time: %lu us", period);

    log_success("BME280 configured successfully");

    return STATUS_OK;
    
}



void bme280_delay_us(uint32_t period, void *intf_ptr){
    sleep_us(period);
}
BME280_INTF_RET_TYPE bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                                      uint32_t length, void *intf_ptr) {
  status_t result = abstract_i2c_write(reg_addr, reg_data, length, *(uint8_t *)intf_ptr);
  if(result != STATUS_OK){
    return BME280_E_COMM_FAIL;
  }
  return BME280_INTF_RET_SUCCESS;
}

BME280_INTF_RET_TYPE bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                                     uint32_t length, void *intf_ptr) {
  status_t result = abstract_i2c_read(reg_addr, reg_data, length, *(uint8_t *)intf_ptr);
  if(result != STATUS_OK){
    return BME280_E_COMM_FAIL;
  }
  return BME280_INTF_RET_SUCCESS;
}

status_t bme280_scan(struct bme280_data* comp_data){
    uint8_t status_reg;

    uint8_t rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, &dev);
    if(rslt != BME280_OK){
        log_error("get reg error",STATUS_SENSOR_ERROR_BME_READ);
        return STATUS_ERROR;
    }
    if(status_reg & BME280_STATUS_MEAS_DONE){
        uint8_t rslt = bme280_get_sensor_data(BME280_TEMP | BME280_HUM | BME280_PRESS,
                               comp_data, &dev);
        if(rslt != BME280_OK){
            log_error("read not ok",STATUS_SENSOR_ERROR_BME_READ);
            return STATUS_SENSOR_ERROR_BME_READ;
        }
    }
    // add prorper erroer handling
    return STATUS_OK;
}