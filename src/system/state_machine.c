#include "state_machine.h"

status_t state_machine_init(state_machine_t *state_machine,
                            system_context_t *ctx) {
  // @Note maybe deafault to assume state_safe?
  state_machine->current_state = STATE_RUN;
  state_machine->previous_state = STATE_RUN;

  state_machine->state_time_ms = 0;

  ctx->sensors_ok = false;
  ctx->power_ok = false;
  ctx->radio_ok = false;
  reset_time(&(state_machine->state_time_ms));
  return STATUS_OK;
}

void state_machine_step(state_machine_t *state_machine, system_context_t *ctx) {

  system_state_t next_state = state_machine->current_state;

  switch (state_machine->current_state) {

  case STATE_RUN:
    if (ctx->critical_fault) {
      next_state = STATE_ERROR;
    }
    if (ctx->low_battery || ctx->power_ok) {
      next_state = STATE_SAFE;
    }
    break;

  case STATE_SAFE:
    if (ctx->critical_fault) {
      next_state = STATE_ERROR;
    }
    if (!ctx->low_battery) {
      next_state = STATE_RUN;
    }
    break;

  case STATE_ERROR:
    // Handle error conditions
    // Transition back to INIT or SAFE as needed
    // now for watchdog
    break;

  default:
    next_state = STATE_ERROR; // Unknown state, transition to ERROR
    break;
  }
  if (next_state != state_machine->current_state) {
    log_info("Changing states from %d to %d after %dms",
             state_machine->current_state, next_state,
             state_machine->state_time_ms);
    state_machine->previous_state = state_machine->current_state;
    state_machine->current_state = next_state;
    state_machine->state_time_ms = 0;
    reset_time(&(state_machine->state_time_ms));
  } else {
    update_time(&(state_machine->state_time_ms));
  }
}