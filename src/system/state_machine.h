#ifndef STATE_MACHINE__H
#define STATE_MACHINE__H

#include "system/status.h"
#include "stdint.h"
#include "stdbool.h"

#include "communication/logging.h"

typedef enum{
    STATE_CHECK,
    STATE_RUN,
    STATE_SAFE,
    STATE_ERROR,
} system_state_t;

typedef struct{
    system_state_t current_state;
    system_state_t previous_state;

    uint32_t state_time_ms;
    
} state_machine_t;

typedef struct{
    bool sensors_ok;
    bool radio_ok;
    bool power_ok;

    bool low_battery;
    bool critical_fault;
    bool gps_found;
    bool radio_connected;
} system_context_t;

status_t state_machine_init(state_machine_t* state_machine, system_context_t* ctx);
void state_machine_step(state_machine_t* state_machine, system_context_t* ctx);

#endif //STATE_MACHINE__H