#ifndef _RADIO__H_
#define _RADIO__H_

#include "system/status.h"
#include "telemetry/packet.h"

status_t init_radio(system_context_t* ctx);
status_t send_packet(packet_t *sys_packet);

#endif // _RADIO__H_