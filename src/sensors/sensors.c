#include "sensors.h"
#include "system/state_machine.h"

status_t init_sensors(sensors_t* sens_handler, system_context_t* ctx){
    ctx->sensors_ok = false;
    configure_bme280(
        &(sens_handler->bme280));
    // configure other sensors
    return STATUS_NOT_IMPLEMENTED;
}

status_t read_sensors(sensors_t* sens_handler, system_context_t* ctx){
    return STATUS_NOT_IMPLEMENTED;
}

status_t sensors_check_health(system_context_t* ctx){
    return STATUS_NOT_IMPLEMENTED;
}