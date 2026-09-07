#include "sensors.h"
#include "sensors/bme280.h"
#include "system/status.h"

status_t init_sensors(sensors_t* sens_handler){
    configure_bme280(
        &(sens_handler->bme280));
    // configure other sensors
    return STATUS_OK; 
}

status_t read_sensors(sensors_t* sens_handler){
    
    return STATUS_OK;
}