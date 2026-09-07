#include "logging.h"

status_t logging_init(void){
    stdio_init_all();
    return STATUS_OK;
}

void log_info(const char* message, ...){
    // opt add enum to str
    va_list args;
    va_start(args, message);

    printf("[\033[34mINFO\033[0m]: ");
    vprintf(message, args);
    printf("\n");

    va_end(args);
}
void log_warning(const char* message){
    printf("[\033[33mWARNING\033[0m]: %s\n", message);
}
void log_error(const char* message, status_t status){
    printf("[\033[31mERROR\033[0m]: %s (Status: %d)\n", message, status);
}

void log_success(const char* message){
    printf("[\033[32mSUCCESS\033[0m]: %s\n", message);
}