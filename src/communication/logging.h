#ifndef LOGGING__H
#define LOGGING__H

#include "system/status.h"
#include "pico/stdio.h"
#include "stdio.h"
status_t logging_init(void);

void log_info(const char* message);
void log_warning(const char* message);
void log_error(const char* message, status_t status);
void log_success(const char* message);

#endif // LOGGING__H