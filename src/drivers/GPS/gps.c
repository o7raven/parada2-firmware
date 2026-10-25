#include "gps.h"
#include "communication/logging.h"
#include "system/status.h"
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <hardware/watchdog.h>

#include <string.h>
#include "nmea.h"
#include "gpgga.h"
#include "gprmc.h"
#include "gpgsa.h"
#include "gpgsv.h"
#include "gpvtg.h"


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

      if (sentence_index < GPS_RX_BUFFER_SIZE - 1) {
        sentence_buffer[sentence_index++] = c;
        sentence_buffer[sentence_index] = '\0';

        size_t length = strlen(sentence_buffer);

        nmea_s *data = nmea_parse(sentence_buffer, length, 1);
if (data != NULL) {

    if (data->type == NMEA_GPGGA) {
        nmea_gpgga_s *gga = (nmea_gpgga_s *)data;

        double latitude =
            gga->latitude.degrees +
            gga->latitude.minutes / 60.0;

        double longitude =
            gga->longitude.degrees +
            gga->longitude.minutes / 60.0;

        if (gga->latitude.cardinal == NMEA_CARDINAL_DIR_SOUTH) {
            latitude = -latitude;
        }

        if (gga->longitude.cardinal == NMEA_CARDINAL_DIR_WEST) {
            longitude = -longitude;
        }

        // Poloha
        gps_handler->lat = (float)latitude;
        gps_handler->lon = (float)longitude;

        // Nadmorska vyska v metrech
        gps_handler->alt = (float)gga->altitude;

        // Pocet pouzitych satelitu a kvalita fixu
        gps_handler->sats_used = (uint8_t)gga->n_satellites;
        gps_handler->fix_qty = (uint8_t)gga->position_fix;

        // UTC cas z GGA
        snprintf(
            gps_handler->time,
            sizeof(gps_handler->time),
            "%02d:%02d:%02d",
            gga->time.tm_hour,
            gga->time.tm_min,
            gga->time.tm_sec
        );

        // GGA fix je platny, pokud je kvalita nenulova
        gps_handler->valid = (gga->position_fix != 0);
        ctx->gps_found = gps_handler->valid;

        log_info("GPS GGA: lat=%.6f lon=%.6f alt=%.1f "
                 "sats=%u fix=%u time=%s",
                 gps_handler->lat, gps_handler->lon, gps_handler->alt,
                 (unsigned)gps_handler->sats_used,
                 (unsigned)gps_handler->fix_qty, gps_handler->time);
    }

    else if (data->type == NMEA_GPRMC) {
      nmea_gprmc_s *rmc = (nmea_gprmc_s *)data;

      // UTC cas z RMC
      snprintf(gps_handler->time, sizeof(gps_handler->time), "%02d:%02d:%02d",
               rmc->date_time.tm_hour, rmc->date_time.tm_min,
               rmc->date_time.tm_sec);

      // Rychlost v uzlech
      gps_handler->knots = (float)rmc->gndspd_knots;

      // RMC obsahuje i polohu
      if (rmc->valid) {
        double latitude = rmc->latitude.degrees + rmc->latitude.minutes / 60.0;

        double longitude =
            rmc->longitude.degrees + rmc->longitude.minutes / 60.0;

        if (rmc->latitude.cardinal == NMEA_CARDINAL_DIR_SOUTH) {
          latitude = -latitude;
        }

        if (rmc->longitude.cardinal == NMEA_CARDINAL_DIR_WEST) {
          longitude = -longitude;
        }

        gps_handler->lat = (float)latitude;
        gps_handler->lon = (float)longitude;

        gps_handler->valid = 1;
      } else {
        gps_handler->valid = 0;
      }

      log_info("GPS RMC: valid=%u speed=%.2f knots time=%s",
               (unsigned)gps_handler->valid, gps_handler->knots,
               gps_handler->time);
    }

    nmea_free(data);

} else {
  log_warning("Invalid NMEA sentence");
}
      }

      sentence_index = 0;
    } else if (sentence_index < GPS_RX_BUFFER_SIZE - 1) {
      sentence_buffer[sentence_index++] = c;
    } else {
      log_warning("GPS sentence buffer overflow");
      sentence_index = 0;
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