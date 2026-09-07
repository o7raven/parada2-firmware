#include "time_increment.h"

void update_time(uint32_t* time_ms){
    *time_ms = to_ms_since_boot(get_absolute_time())-time_snapshot_ms;
}
void reset_time(uint32_t* time_ms){
    *time_ms = 0;
    time_snapshot_ms = to_ms_since_boot(get_absolute_time());
}