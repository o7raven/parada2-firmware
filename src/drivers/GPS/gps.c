#include "gps.h"
#include "system/status.h"
#include <hardware/gpio.h>
#include <hardware/uart.h>


status_t gps_init(gps_t* gps_handler, system_context_t* ctx){
    ctx->gps_found = false;
    gps_handler->lat = 0;
    gps_handler->lon = 0;
    gps_handler->data_age = 0;
    gps_handler->month = 0;
    gps_handler->day= 0;
    gps_handler->hour= 0;
    gps_handler->minute= 0;
    gps_handler->second= 0;
    gps_handler->milisecond= 0;

    uart_init(GPS_UART, GPS_BAUD);
    gpio_set_function(GPS_TX, GPIO_FUNC_UART);
    gpio_set_function(GPS_RX, GPIO_FUNC_UART);

    uart_set_baudrate(GPS_UART, GPS_BAUD);
    return STATUS_OK;
}


status_t get_location(gps_t* gps_handler, system_context_t* ctx){
    while(uart_is_readable(GPS_UART)){
        char c = uart_getc(GPS_UART);

        // implement nmea parsing (with external libary)
    }
    return STATUS_NOT_IMPLEMENTED;
}