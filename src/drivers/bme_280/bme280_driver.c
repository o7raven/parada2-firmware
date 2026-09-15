#include "bme280_driver.h"
#include "system/status.h"




status_t configure_bme280(bme280_t* bme280_handler){
    dev.intf = BME280_I2C_INTF;

    bme280_init(&dev);

    return STATUS_NOT_IMPLEMENTED;
}

status_t read_bme(bme280_t* bme280_handler){
    return STATUS_NOT_IMPLEMENTED;
}