#ifndef _RADIO__H_
#define _RADIO__H_

#include "system/status.h"
#include "telemetry/packet.h"

#include "telemetry/SX1278_hw.h"
#include "telemetry/SX1278.h"
#include "config/hardware_config.h"

status_t radio_init(SX1278_hw_t *hw, SX1278_t *sx, system_context_t* ctx);
status_t send_packet(SX1278_t *sx, packet_t *sys_packet);

#endif // _RADIO__H_