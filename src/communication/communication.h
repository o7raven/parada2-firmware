#ifndef COMMUNICATION__H
#define COMMUNICATION__H

#include "system/status.h"

#include "drivers/UART/uart.h"
#include "drivers/SPI/spi.h"
#include "drivers/i2c/i2c.h"

status_t communication_init(void);

#endif //COMMUNICATION__H