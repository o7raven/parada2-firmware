#ifndef _GPS__H
#define _GPS__H

#include "system/status.h"
#include "inttypes.h"
#include "system/state_machine.h"

typedef struct{
    uint8_t valid;
    float lon, lat;
    unsigned long data_age;
    uint8_t month, day, hour, minute, second, milisecond;

} gps_t;

status_t gps_init(gps_t* gps_handler, system_context_t* ctx);
status_t get_location(gps_t* gps_handler, system_context_t* ctx);


#endif // _GPS__H