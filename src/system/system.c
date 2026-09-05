#include "system.h"

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



    return STATUS_OK;
}