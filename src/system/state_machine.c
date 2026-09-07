#include "state_machine.h"

status_t state_machine_init(state_machine_t *state_machine,
                            system_context_t *ctx) {
  state_machine->current_state = STATE_CHECK;
  state_machine->previous_state = STATE_CHECK;

  state_machine->state_time_ms = 0;

  ctx->sensors_ok = false;
  ctx->power_ok = false;
  ctx->radio_ok = false;

  return STATUS_OK;
}

void state_machine_step(
    state_machine_t* state_machine,
    system_context_t* ctx
) {

    system_state_t next_state = state_machine->current_state;
    
    switch (state_machine->current_state) {
        case STATE_CHECK:
          state_machine->previous_state = state_machine->current_state;
          if (ctx->sensors_ok && ctx->radio_ok &&
              ctx->power_ok) {
            next_state = STATE_RUN;
          } else {
            next_state = STATE_SAFE;
          }
            break;

        case STATE_RUN:
          state_machine->previous_state = state_machine->current_state;
          if(ctx->critical_fault){
            next_state = STATE_ERROR;
          }
          if(ctx->low_battery){
            next_state = STATE_SAFE;
          }
          break;

        case STATE_SAFE:
            if(ctx->critical_fault){
                next_state = STATE_ERROR;
            }
            if(ctx->sensors_ok && ctx->radio_ok && ctx->power_ok){
                next_state = STATE_RUN;
            }
            if(!ctx->low_battery && ctx->power_ok){
                next_state = STATE_RUN;
            }
            break;

        case STATE_ERROR:
            // Handle error conditions
            // Transition back to INIT or SAFE as needed
            break;

        default:
            next_state = STATE_ERROR; // Unknown state, transition to ERROR
            break;

        }
        if (next_state != state_machine->current_state) {
          state_machine->previous_state = state_machine->current_state;
          state_machine->current_state = next_state;
          state_machine->state_time_ms = 0;
        }
}