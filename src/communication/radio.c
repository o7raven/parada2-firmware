#include "radio.h"
#include "communication/logging.h"
#include "system/state_machine.h"
#include "system/status.h"
#include <sys/types.h>

packet_t create_packet(sensors_t* sensors, gps_t* gps, system_context_t* ctx){
    log_info("Creating a packet");
    packet_t _packet;

    return _packet;
}


status_t send_data(sensors_t* _s_handler, gps_t* _gps_handler, system_context_t* _ctx_handler){
    packet_t packet_to_send = create_packet(_s_handler, _gps_handler, _ctx_handler);
    send_packet(&packet_to_send);

    return STATUS_NOT_IMPLEMENTED;
}

status_t init_radio(system_context_t* ctx){
    ctx->radio_ok = false;
    ctx->radio_connected = false;

    return STATUS_NOT_IMPLEMENTED;
}
//temp var for testing
static int packet_id = 0;
status_t send_packet(packet_t* create_packet){
    packet_id++;
    log_info("{%d} Sending a packet", packet_id);
    return STATUS_NOT_IMPLEMENTED;

}