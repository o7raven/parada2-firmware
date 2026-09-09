#ifndef _BME280__H
#define _BME280__H

#include "system/status.h"

typedef struct{
    float pressure;
    float humidity;
    float temperature;
} bme280_t;

status_t configure_bme280(bme280_t* bme280_handler);
status_t read_bme(bme280_t* bme280_handler);


#endif // _BME280__H