#ifndef _TIME_INCREMENT__H
#define _TIME_INCREMENT__H

#include "pico/time.h"

static uint32_t time_snapshot_ms=0;
void update_time(uint32_t* time_ms);
void reset_time(uint32_t* time_ms);

#endif // _TIME_INCREMENT__H