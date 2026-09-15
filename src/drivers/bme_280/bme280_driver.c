#include "bme280_driver.h"
#include "communication/logging.h"
#include "drivers/i2c/i2c.h"
#include "libs/bosch/bme280.h"
#include "libs/bosch/bme280_defs.h"
#include "system/status.h"
#include <pico/time.h>
#include <stdint.h>



// @Note add proper error handling
void configure_bme280(){
    uint8_t rslt;

    struct bme280_settings settings;
    log_info("Configuring bme280 ...");
    

    dev.read = bme280_i2c_read;
    dev.write = bme280_i2c_write;

    uint8_t dev_addr = BME280_I2C_ADDR_PRIM;
    dev.intf = dev_addr;
    dev.intf_ptr = &dev_addr;

    dev.delay_us = bme280_delay_us;

    //sleep before config
    rslt = bme280_init(&dev);
    if(rslt!= BME280_OK){
        log_error("BME280 initialization failed", STATUS_SENSOR_ERROR_BME_CONFIG);
        return;
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
        return;
    }

    rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);

    if(rslt != BME280_OK){
        log_error("Failed to SET BME280 sensor mode", STATUS_SENSOR_ERROR_BME_CONFIG);
        return;
    }

    rslt = bme280_cal_meas_delay(&period, &settings);

    if(rslt != BME280_OK){
        log_error("Failed to calculate measurement delay", STATUS_SENSOR_ERROR_BME_CONFIG);
        return;
    }

    log_info("Measurement time: %lu us", period);

    log_success("BME280 configured successfully");
    
}



void bme280_delay_us(uint32_t period, void *intf_ptr){
    sleep_us(period);
}
BME280_INTF_RET_TYPE bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                                      uint32_t length, void *intf_ptr) {
  abstract_i2c_write(reg_addr, reg_data, length, *(uint8_t *)intf_ptr);
  return BME280_INTF_RET_SUCCESS;
}

BME280_INTF_RET_TYPE bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                                     uint32_t length, void *intf_ptr) {
  abstract_i2c_read(reg_addr, reg_data, length, *(uint8_t *)intf_ptr);
  return BME280_INTF_RET_SUCCESS;
}

void bme280_scan(struct bme280_data* comp_data){
    uint8_t status_reg;

    bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, &dev);
    if(status_reg & BME280_STATUS_MEAS_DONE){
        bme280_get_sensor_data(BME280_TEMP | BME280_HUM | BME280_PRESS,
                               comp_data, &dev);
    }
}