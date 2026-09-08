#ifndef _RADIO__H_

#include "system/status.h"
#include "sensors/sensors.h"


typedef struct{

} packet_t;

packet_t create_packet(void);
status_t data_send(void);

#define _RAIDIO__H_
#endif // _RADIO__H_