#ifndef __PACKET_H__
#define __PACKET_H__

#include "drivers/GPS/gps.h"

#define CRC_INIT_VAL 0xFFFF
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

uint16_t calc_crc16(packet_t *_p);
#endif // PACKET HEADER