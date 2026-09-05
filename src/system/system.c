#include "system.h"

// TODO: Implement a custom error status for each error type
status_t system_init(void){
    status_t init_status = STATUS_OK;

    logging_init();
    log_info("Logging initialization successful");
    log_info("Starting system initialization ...");

    log_info("Info log working");
    log_warning("Warning log working");
    log_error("Error log working", STATUS_DUMMY_ERROR);

    init_status = blink_init();
    if(init_status != STATUS_OK){
        log_error("Blink initialization failed", init_status);
        return STATUS_ERROR;
    }
    log_info("Blink initialization successful");
    blink(300, 3);

    init_status = communication_init();
    if(init_status != STATUS_OK){
        log_error("Communication initialization failed", init_status);
        return STATUS_ERROR;
    }
    log_info("Communication initialization successful");


    init_status = state_machine_init(&system_state_machine, &system_ctx);
    if(init_status != STATUS_OK){
        log_error("State machine initialization failed", init_status);
        return STATUS_ERROR;
    }
    log_info("State machine initialization successful");

    log_success("Initialization complete");

    return STATUS_OK;
}

void system_run(void){
    log_info("Starting system run loop ...");
    while(1){
        switch(system_state_machine.current_state){
        case STATE_CHECK:
                // Perform checks and update system context
                break;
            case STATE_RUN:
                // Perform normal operations
                break;
            case STATE_SAFE:
                // Perform safe mode operations (Prob just less frequent data collection and telemtry)
                break;
            case STATE_ERROR:
                // Handle error conditions
                break;
            default:
                log_error("Unknown state encountered", STATUS_ERROR);
                break;
        }
        // Update system context here based on sensor readings, communication status, etc.

        state_machine_step(&system_state_machine, &system_ctx);

    }
}