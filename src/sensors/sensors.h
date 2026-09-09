#ifndef _SENSORS__H
#define _SENSORS__H

#include "drivers/bme_280/bme280.h"
#include "system/status.h"
#include "system/state_machine.h"

typedef struct {
   bme280_t bme280; 
} sensors_t;

status_t init_sensors(sensors_t* sens_handler, system_context_t* ctx);

status_t read_sensors(sensors_t* sens_handler, system_context_t* ctx);

status_t sensors_check_health(system_context_t* ctx);

#endif // _SENSORS__H