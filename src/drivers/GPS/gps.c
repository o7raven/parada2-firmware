#include "gps.h"
#include "communication/logging.h"
#include "system/status.h"
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <hardware/watchdog.h>


static volatile uint8_t gps_rx_buffer[GPS_RX_BUFFER_SIZE];

static volatile uint16_t gps_rx_index = 0;
static volatile uint16_t gps_rx_tail = 0;
static volatile bool gps_rx_overflow = false;



status_t gps_init(gps_t* gps_handler, system_context_t* ctx){
    ctx->gps_found = false;
    gps_handler->lat = 0;
    gps_handler->lon = 0;
    gps_handler->alt = 0;
    gps_handler->valid = 0;
    gps_handler->time[0] = '\0';
    gps_handler->hdop = 0;
    gps_handler->vdop = 0;
    gps_handler->pdop = 0;
    gps_handler->sats_used = 0;
    gps_handler->sats_in_view = 0;
    gps_handler->fix_qty = 0;
    gps_handler->fix_type = 0;

    uint rslt = uart_init(GPS_UART, GPS_BAUD);
    if(rslt != GPS_BAUD){
        log_warning("GPS UART initialization failed");
    }
    gpio_set_function(GPS_TX, GPIO_FUNC_UART);
    gpio_set_function(GPS_RX, GPIO_FUNC_UART);

    uart_set_hw_flow(GPS_UART, false, false);

    irq_set_exclusive_handler(GPS_UART_IRQ, gps_uart_irq_handler);
    irq_set_enabled(GPS_UART_IRQ, true);
    uart_set_irq_enables(GPS_UART, true, false);

    // gps from implementation from
    // temporary FIX
    /* https://github.com/DragonflyValkyrie/pico-gps/blob/main/pico-gps.c */
    const char configurations[] =
        "$PMTK314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0*2C\r\n";

    uart_puts(GPS_UART, configurations);

    return STATUS_OK;
}

status_t get_location(gps_t *gps_handler, system_context_t *ctx) {

  static char sentence_buffer[GPS_RX_BUFFER_SIZE];
  static uint16_t sentence_index = 0;
  uint8_t byte;

  while (gps_read_byte(&byte)) {
    char c = (char)byte;
    if (c == '$') {
      sentence_index = 0;
      sentence_buffer[sentence_index++] = c;
    } else if (c == '\n') {

      sentence_buffer[sentence_index] = '\0';
      log_info("Received GPS sentence: %s", sentence_buffer);
      sentence_index = 0;
    } else {
      if (sentence_index < GPS_RX_BUFFER_SIZE - 1) {
        sentence_buffer[sentence_index++] = c;
      } else {
        log_warning("GPS sentence buffer overflow");
        sentence_index = 0;
      }
    }
  }
  return STATUS_OK;
}

static bool gps_read_byte(uint8_t *byte) {
  if (gps_rx_tail == gps_rx_index) {
    return false;
  }
  *byte = gps_rx_buffer[gps_rx_tail];
  gps_rx_tail = (gps_rx_tail + 1) % GPS_RX_BUFFER_SIZE;
  return true;
}

static void gps_uart_irq_handler(void) {
  gps_rx_overflow = false;
  while (uart_is_readable(GPS_UART)) {
    uint8_t gps_byte = uart_getc(GPS_UART);
    uint16_t next_index = (gps_rx_index + 1) % GPS_RX_BUFFER_SIZE;
    if (next_index != gps_rx_tail) {
      gps_rx_buffer[gps_rx_index] = gps_byte;
      gps_rx_index = next_index;
    } else {
      gps_rx_overflow = true;
      log_warning("GPS RX buffer overflow");
    }
  }
}