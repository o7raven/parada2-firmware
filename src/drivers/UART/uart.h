#ifndef UART__H
#define UART__H

#include "system/status.h"
#include "config/hardware_config.h"

#include "hardware/uart.h"
#include "hardware/gpio.h"

status_t abstract_uart_init(void);

#endif //UART__H