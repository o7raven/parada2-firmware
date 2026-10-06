#ifndef _GPS__H
#define _GPS__H

#include "system/status.h"
#include "inttypes.h"
#include "system/state_machine.h"
#include "hardware/uart.h"
#include "config/hardware_config.h"

typedef struct{
    uint8_t valid;
    float lon, lat, alt;
    char time[16];
    float hdop,vdop,pdop;
    uint8_t sats_used, sats_in_view, fix_qty;
    uint8_t fix_type;
    float knots;


} gps_t;

status_t gps_init(gps_t* gps_handler, system_context_t* ctx);
status_t get_location(gps_t* gps_handler, system_context_t* ctx);
static void gps_uart_irq_handler(void);
static bool gps_read_byte(uint8_t* byte);


#endif // _GPS__H