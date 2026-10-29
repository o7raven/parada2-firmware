#include "system.h"
#include "communication/logging.h"
#include "communication/microsd_layer.h"
#include "communication/radio.h"
#include "config/system_config.h"
#include "system/state_machine.h"
#include "system/status.h"
#include <hardware/watchdog.h>
#include <pico/time.h>

#include "telemetry/packet.h"

#if SD_LOGGING_ENABLED
#include "communication/microsd_layer.c"
#endif

volatile bool telemetry_is_due = false;
repeating_timer_t telemetry_timer;
bool telemetry_callback_is_running = false;

status_t system_init(void) {

  // @Note debugging purposes (remove later)
  system_ctx.power_ok = true;
  system_ctx.low_battery = false;
  system_ctx.critical_fault = false;

  status_t init_status = STATUS_OK;

  telemetry_is_due = false;
  telemetry_callback_is_running = false;

  logging_init();
  sleep_ms(BOOT_TIME_TO_INIT_ms);
  log_info("Info log working");
  log_warning("Warning log working");
  log_error("Error log working", STATUS_DUMMY_ERROR);
  log_success("Success log working");

  if (watchdog_caused_reboot()) {
    log_warning("Watchdog has caused a reboot!");
    sleep_ms(500);
  }

  log_info("Starting system initialization ...");
  sleep_ms(BOOT_TIME_TO_INIT_ms);

  init_status = communication_init();
  if (init_status != STATUS_OK) {
    log_error("Communication initialization failed", init_status);
    return init_status;
  } else {
    log_success("Communication initialization successful");
  }

#if SD_LOGGING_ENABLED
  init_status = microsd_init();
  if (init_status != STATUS_OK) {
    log_error("MicroSD initialization failed", STATUS_ERROR);
  } else {
    log_success("MicroSD initialization successful");
  }
#endif

  sleep_ms(BOOT_TIME_TO_INIT_ms);

  init_status = init_sensors(&sensors, &system_ctx.sensors);
  if (init_status != STATUS_OK) {
    log_warning("Sensors initialization failed");
  } else {
    log_success("Sensors initialization successful");
  }
  sleep_ms(BOOT_TIME_TO_INIT_ms);

  init_status = gps_init(&gps_data, &system_ctx);
  if (init_status != STATUS_OK) {
    log_error("GPS UART initialization failed", init_status);
  } else {
    log_success("GPS UART initialization successful");
  }

  sleep_ms(BOOT_TIME_TO_INIT_ms);

  init_status = state_machine_init(&system_state_machine);
  if (init_status != STATUS_OK) {
    log_error("State machine initialization failed", init_status);
  } else {
    log_success("State machine initialization successful");
  }

  sleep_ms(BOOT_TIME_TO_INIT_ms);
  // @Watchdog
  watchdog_enable(WATCHDOG_TIMEOUT_ms + DATA_COLLECTION_PERIOD_ms,
                  WATCHDOG_PAUSE_ON_DBG);

  log_success("Initialization complete");
  log_context(&system_ctx);
  log_info("State machine starting ... ");

  return STATUS_OK;
}

bool telemetry_callback(struct repeating_timer *t) {
  telemetry_is_due = true;
  return true;
}
void create_telemetry_timer(system_state_t state) {
  if (telemetry_callback_is_running) {
    log_info("Callback is already running, cancelling");
    cancel_repeating_timer(&telemetry_timer);
  }
  log_info("Adding a repeating timer in state: %d", state);
  switch (state) {
    // duplicity fix later
  case STATE_RUN:
    telemetry_callback_is_running =
        add_repeating_timer_ms(RUN_STATE_PACKET_FREQUENCY_ms,
                               &telemetry_callback, false, &telemetry_timer);
    break;
  case STATE_SAFE:
    telemetry_callback_is_running =
        add_repeating_timer_ms(SAFE_STATE_PACKET_FREQUENCY_ms,
                               &telemetry_callback, false, &telemetry_timer);
    break;
  case STATE_ERROR:
    cancel_repeating_timer(&telemetry_timer);
    telemetry_callback_is_running = false;
    break;
  default:
    log_warning("Entered unknown state");
    break;
  }
  if (telemetry_callback_is_running) {
    log_success("Interrupt created sucessfully");
  } else {
    log_error("There has been an error creating the interrupt", STATUS_ERROR);
  }
}

void system_run(void) {
  log_info("Starting system run loop ...");

  packet_t system_packet;
  while (1) {
    if (state_machine_changed()) {
      create_telemetry_timer(system_state_machine.current_state);
    }

    // @Note create a task with instead of log every loop
    log_info("In [%d] for %d ms", system_state_machine.current_state,
             system_state_machine.state_time_ms);

    // @Note : state_machine_step() only makes decisions about states
    state_machine_step(&system_state_machine, &system_ctx);
    system_work();

    if (telemetry_is_due) {
      log_info("Telemetry transmission is due");
      telemetry_is_due = false;
      // send_data(&sensors, &gps_data, &system_ctx);

      // @Note not good implementation change asap
      packet_t telemetry_packet = create_packet(
          &system_state_machine, &system_ctx, &sensors, &gps_data);
      log_info("Packet no. %d has been created!", telemetry_packet.packet_id);
      status_t t_status = send_packet(&telemetry_packet);
      if (t_status != STATUS_OK) {
        log_warning("Radio packet has failed");
      }
#if SD_LOGGING_ENABLED
      if (write_packet(&telemetry_packet) != STATUS_OK) {
        log_warning("write_packet returned an error!");
      }
#endif
    }

    // @Watchdog
    sleep_ms(DATA_COLLECTION_PERIOD_ms);

    watchdog_update();
  }
}

void system_work(void) {
  read_sensors(&sensors, &system_ctx.sensors);
  get_location(&gps_data, &system_ctx);
}