#ifndef SYSTEM__H
#define SYSTEM__H

#include "status.h"
#include "sensors/sensors.h"

#include "misc/blink.h"
#include "communication/communication.h"
#include "communication/logging.h"
#include "system/state_machine.h"
#include "config/system_config.h"
#include "communication/radio.h"
#include "drivers/GPS/gps.h"

#include "hardware/watchdog.h"
#include <pico/time.h>

static state_machine_t system_state_machine;
static system_context_t system_ctx;
static sensors_t sensors;
static gps_t gps_data;

static volatile bool radio_is_due;
static repeating_timer_t radio_timer;
static bool radio_callback_is_running;

/* @brief Initializes the system and its components
 * @return STATUS_OK if initialization was successful, otherwise returns an error status
 * @note This function should be 
 *       called before any other
 *       system functions are used and should only be called once.
 */


status_t system_init(void);

void system_work(void);

void system_run(void);

void create_radio_timer(system_state_t state);

bool radio_callback(struct repeating_timer *t);

#endif //SYSTEM__H