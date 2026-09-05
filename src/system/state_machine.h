#ifndef STATE_MACHINE__H
#define STATE_MACHINE__H

#include "system/status.h"

typedef enum{
    STATE_INIT,
    STATE_CHECK,
    STATE_RUN,
    STATE_SAFE,
    STATE_ERROR,
} system_state_t;

typedef struct{
    system_state_t current_state;
    system_state_t previous_state;
} state_machine_t;

status_t state_machine_init(state_machine_t* state_machine);
system_state_t state_machine_step(state_machine_t* state_machine);

#endif //STATE_MACHINE__H