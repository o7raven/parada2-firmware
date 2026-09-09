#ifndef _GPS__H
#define _GPS__H

#include "system/status.h"
#include "inttypes.h"

typedef struct{
    uint8_t valid;
    float lon, lat;
    unsigned long data_age;
    uint8_t month, day, hour, minute, second, milisecond;

} gps_t;

status_t get_location(gps_t* gps_handler);

#endif // _GPS__H