#include "system.h"

// TODO: Implement a custom error status for each error type
status_t system_init(void) {
  status_t init_status = STATUS_OK;

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
  if(init_status != STATUS_OK){
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
  watchdog_enable(WATCHDOG_TIMEOUT_MS, WATCHDOG_PAUSE_ON_DBG);

  log_success("Initialization complete");
  log_info("State machine starting ... ");

  return STATUS_OK;
}

void system_run(void) {
  log_info("Starting system run loop ...");
  while (1) {
    // check health
    // @Note create a task with instead of log every loop
    log_info("In [%d] for %d ms", system_state_machine.current_state,
             system_state_machine.state_time_ms);
    // Update system context here based on sensor readings, communication
    // status, etc. communication, power, sensors with ctx




    // @Note : state_machine_step() only makes decisions about states
    state_machine_step(&system_state_machine, &system_ctx);

    switch (system_state_machine.current_state) {

    // @Note : each func updates its own context -> removes the need for a global update_context function
    case STATE_RUN:
      read_sensors(&sensors, &system_ctx);
      get_location(&gps_data, &system_ctx);
      data_send(&sensors, &gps_data, &system_ctx);
      break;


    case STATE_SAFE:
      // Perform safe mode operations (Prob just less frequent data collection
      // and telemtry)
      break;



    case STATE_ERROR:
      if (system_state_machine.state_time_ms > 30 * 1000) {
        watchdog_reboot(0, 0, 500);
      }
      break;






    default:
      log_error("Unknown state encountered", STATUS_ERROR);
      break;
    }


    // @Watchdog
    watchdog_update();
  }
}