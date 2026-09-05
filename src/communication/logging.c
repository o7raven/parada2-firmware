#include "logging.h"

status_t logging_init(void){
    stdio_init_all();
    return STATUS_OK;
}

void log_info(const char* message){
    printf("[INFO]: %s\n", message);
}
void log_warning(const char* message){
    printf("[WARNING]: %s\n", message);
}
void log_error(const char* message, status_t status){
    printf("[ERROR]: %s (Status: %d)\n", message, status);
}