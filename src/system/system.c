#include "system.h"
#include "communication/logging.h"
#include "communication/radio.h"
#include "config/system_config.h"
#include "system/state_machine.h"
#include <pico/time.h>

// TODO: Implement a custom error status for each error type
status_t system_init(void) {
  status_t init_status = STATUS_OK;

  radio_is_due = false;
  radio_callback_is_running = false;

  logging_init();
  log_info("Info log working");
  log_warning("Warning log working");
  log_error("Error log working", STATUS_DUMMY_ERROR);
  log_success("Success log working");

  log_info("Starting system initialization ...");

  init_status = communication_init();
  if (init_status != STATUS_OK) {
    log_error("Communication initialization failed", init_status);
    return init_status;
  }
  log_success("Communication initialization successful");

  init_status = init_sensors(&sensors, &system_ctx);
  if (init_status != STATUS_OK) {
    log_error("Sensors initialization failed", init_status);
    return init_status;
  }
  log_success("Sensors initialization successful");

  init_status = gps_init(&gps_data, &system_ctx);
  if (init_status != STATUS_OK) {
    log_error("GPS initialization failed", init_status);
    return init_status;
  }

  init_status = state_machine_init(&system_state_machine);
  if (init_status != STATUS_OK) {
    log_error("State machine initialization failed", init_status);
    return init_status;
  }
  log_success("State machine initialization successful");

  // @Watchdog
  watchdog_enable(WATCHDOG_TIMEOUT_ms+DATA_COLLECTION_PERIOD_ms, WATCHDOG_PAUSE_ON_DBG);

  log_success("Initialization complete");
  log_info("State machine starting ... ");

  return STATUS_OK;
}

bool radio_callback(struct repeating_timer *t) {
  radio_is_due = true;
  return true;
}
void create_radio_timer(system_state_t state) {
  if (radio_callback_is_running) {
    log_info("Callback is already running, cancelling");
    cancel_repeating_timer(&radio_timer);
  }
  log_info("Adding a repeating timer in state: %d", state);
  switch (state) {
  case STATE_RUN:
    add_repeating_timer_ms(RUN_STATE_PACKET_FREQUENCY_ms, &radio_callback,
                           false, &radio_timer);
    break;
  case STATE_SAFE:
    add_repeating_timer_ms(SAFE_STATE_PACKET_FREQUENCY_ms, &radio_callback,
                           false, &radio_timer);
    break;
  case STATE_ERROR:
    cancel_repeating_timer(&radio_timer);
    radio_callback_is_running = false;
    break;
  default:
    break;
  }
}

void system_run(void) {
  log_info("Starting system run loop ...");

  while (1) {
    if(state_machine_changed()){
      create_radio_timer(system_state_machine.current_state);
    }

    // @Note create a task with instead of log every loop
    log_info("In [%d] for %d ms", system_state_machine.current_state,
             system_state_machine.state_time_ms);

    // @Note : state_machine_step() only makes decisions about states
    state_machine_step(&system_state_machine, &system_ctx);
    system_work();

    if(radio_is_due){
      log_info("Radio transmission is due");
      radio_is_due = false;
      send_data(&sensors, &gps_data, &system_ctx);
    }

    // @Watchdog
    sleep_ms(DATA_COLLECTION_PERIOD_ms);

    watchdog_update();
  }
}


void system_work(void){
      read_sensors(&sensors, &system_ctx);
      get_location(&gps_data, &system_ctx);
}