#include "gps.h"
#include "communication/logging.h"
#include "system/status.h"
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <hardware/watchdog.h>


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

    uint rslt = uart_init(GPS_UART, GPS_BAUD);
    if(rslt != GPS_BAUD){
        log_warning("GPS UART initialization failed");
    }
    gpio_set_function(GPS_TX, GPIO_FUNC_UART);
    gpio_set_function(GPS_RX, GPIO_FUNC_UART);

    uart_set_hw_flow(GPS_UART, false, false);
    uart_set_baudrate(GPS_UART, GPS_BAUD);

    // gps from implementation from
    /* https://github.com/DragonflyValkyrie/pico-gps/blob/main/pico-gps.c */
    const char configurations[] =
        "$PMTK314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0*2C\r\n";

    uart_puts(GPS_UART, configurations);

    return STATUS_OK;
}

status_t get_location(gps_t *gps_handler, system_context_t *ctx) {
  static char sentence_buffer[256];
  static uint16_t sentence_index = 0;

  while (uart_is_readable(GPS_UART)) {

    char data = uart_getc(GPS_UART);

    if (data == '$') {
      sentence_index = 0;
    }

    if (sentence_index < sizeof(sentence_buffer) - 1) {
      sentence_buffer[sentence_index++] = data;
    }

    if (data == '\n') {
      sentence_buffer[sentence_index] = '\0';

      printf("%s", sentence_buffer);

      sentence_index = 0;
    }
  }

  return STATUS_NOT_IMPLEMENTED;
}