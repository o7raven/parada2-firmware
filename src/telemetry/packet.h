#ifndef __PACKET_H__
#define __PACKET_H__

#include "drivers/GPS/gps.h"
#include "drivers/hmc5883l/hmc5883l.h"
#include "sensors/sensors.h"
#include "system/state_machine.h"
typedef struct __attribute__((packed)) {
  // uint32 could potentionally hold packet ids for 24 days
  uint32_t packet_id;
  state_machine_t state_m;
  system_context_t system_ctx;

  sensors_t sensors;

  gps_t gps_data;

  uint16_t crc16;

} packet_t;
// @Note gps_data needs to be parsed beforehand - TODO
packet_t create_packet(state_machine_t *state, system_context_t *sys_ctx,
                       sensors_t *sensors, gps_t *gps_data);
#endif // PACKET HEADER