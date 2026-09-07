#ifndef _SENSORS__H
#define _SENSORS__H

#include "system/state_machine.h"
#include "system/status.h"
#include "bme280.h"

typedef struct {
   bme280_t bme280; 
} sensors_t;

status_t init_sensors(sensors_t* sens_handler);

status_t read_sensors(sensors_t* sens_handler, system_context_t* ctx);

#endif // _SENSORS__H