#include "packet.h"
#include "system/state_machine.h"
#include <stdint.h>

static uint32_t _p_id=0;
packet_t create_packet(state_machine_t *state ,system_context_t *sys_ctx, sensors_t *sensors, gps_t* gps_data){
    packet_t packet;

    packet.packet_id = _p_id++;

    packet.state_m = *state;
    packet.system_ctx = *sys_ctx;
    packet.sensors = *sensors;
    packet.gps_data = *gps_data;
    packet.crc16 = 420;


    
    return packet;
}