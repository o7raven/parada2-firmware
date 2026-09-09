#ifndef _RADIO__H_

#include "system/status.h"
#include "sensors/sensors.h"
#include "drivers/GPS/gps.h"
#include "sensors/sensors.h"


typedef struct{

} packet_t;

packet_t create_packet(void);
status_t data_send(sensors_t* _s_handler, gps_t* _gps_handler);

#define _RAIDIO__H_
#endif // _RADIO__H_