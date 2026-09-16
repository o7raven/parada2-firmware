#include "logging.h"
#include "config/system_config.h"
#include "misc/blink.h"

status_t logging_init(void){
    stdio_init_all();
    blink_init();
    blink(300, 3);
    return STATUS_OK;
}

// @Note : add leds that represent log status
void log_info(const char* message, ...){
    // opt add enum to str
    va_list args;
    va_start(args, message);

    printf("[\033[34mINFO\033[0m]: ");
    vprintf(message, args);
    printf("\n");

    va_end(args);

    blink(LOG_BLINK_ms, 1);
}
void log_warning(const char* message){
    printf("[\033[33mWARNING\033[0m]: %s\n", message);
    blink(LOG_BLINK_ms, 1);
}
void log_error(const char* message, status_t status){
    printf("[\033[31mERROR\033[0m]: %s (Status: %d)\n", message, status);
    blink(LOG_BLINK_ms, 1);
}

void log_success(const char* message){
    printf("[\033[32mSUCCESS\033[0m]: %s\n", message);
    blink(LOG_BLINK_ms, 1);
}

void log_sensor(const char* message, ...){
    va_list args;
    va_start(args, message);

    printf("[\033[36mSENSOR DATA\033[0m]: ");
    vprintf(message, args);
    printf("\n");

    va_end(args);

    blink(LOG_BLINK_ms, 1);
}

void log_context(system_context_t *ctx) {

    // Awful formatting ikr
  printf(
      "[\033[35mCONTEXT\033[0m]\n\tsensors_ok:\t%d\n\tradio_ok:\t"
      "%d\n\tpower_ok:\t%d\n\tlow_battery:\t%d\n\tcritical_fault:\t%d\n\tgps_"
      "found:\t%d\n\tradio_connected:\t%d\n",
      ctx->sensors_ok, ctx->radio_ok, ctx->power_ok, ctx->low_battery,
      ctx->critical_fault, ctx->gps_found, ctx->radio_connected);
  blink(LOG_BLINK_ms, 1);
}