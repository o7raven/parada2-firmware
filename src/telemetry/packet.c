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
    packet.crc16 = calc_crc16(&packet);


    
    return packet;
}



uint16_t calc_crc16(packet_t *_p){
    uint16_t _crc = CRC_INIT_VAL;
    size_t packet_size = sizeof(*_p) - 2;
    const uint8_t *byte = (const uint8_t* ) _p;
    for(size_t i = 0; i < packet_size; i++){
        _crc ^= byte[i];

        for(uint8_t bit = 0; bit < 8; bit++){
           if(_crc & 0x0001){
             _crc = (_crc >> 1) ^ 0xA001;
           }else{
            _crc = _crc >> 1;
           } 
        }
    }
    return _crc;

}
