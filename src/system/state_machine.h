#ifndef STATE_MACHINE__H
#define STATE_MACHINE__H

typedef enum{
    STATE_INIT,
    STATE_CHECK,
    STATE_RUN,
    STATE_SENSOR,
    STATE_TELEMETRY,
    STATE_SAFE,
    STATE_ERROR,
} system_state_t;

system_state_t state_machine_step(system_state_t current_state);

#endif //STATE_MACHINE__H