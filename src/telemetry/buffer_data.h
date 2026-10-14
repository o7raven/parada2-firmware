#ifndef __BUFFER_DATA_H__
#define __BUFFER_DATA_H__
#include "system/status.h"
#include "telemetry/packet.h"
#include "stdio.h"
status_t buffer_data(char* buffer, packet_t *current_packet);
#endif