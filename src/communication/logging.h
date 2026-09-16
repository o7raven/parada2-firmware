#ifndef LOGGING__H
#define LOGGING__H

#include "system/state_machine.h"
#include "system/status.h"
#include "pico/stdio.h"
#include "misc/blink.h"
#include "stdio.h"
status_t logging_init(void);

void log_info(const char* message, ...);
void log_warning(const char* message);
void log_error(const char* message, status_t status);
void log_success(const char* message);

void log_sensor(const char* message, ...);
void log_context(system_context_t* ctx);

#endif // LOGGING__H
