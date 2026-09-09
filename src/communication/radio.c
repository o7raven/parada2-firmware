#include "radio.h"
#include "system/status.h"

packet_t create_packet(void){
}


status_t data_send(sensors_t* _s_handler, gps_t* _gps_handler){
    return STATUS_NOT_IMPLEMENTED;
}

status_t init_radio(system_context_t* ctx){
    ctx->radio_ok = false;
    ctx->radio_connected = false;

    return STATUS_NOT_IMPLEMENTED;
}