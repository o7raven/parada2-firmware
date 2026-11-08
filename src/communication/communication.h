#ifndef COMMUNICATION__H
#define COMMUNICATION__H

#include "system/status.h"

#include "drivers/UART/uart.h"
#include "drivers/SPI/spi.h"
#include "drivers/i2c/i2c.h"
#include "telemetry/SX1278_hw.h"
#include "telemetry/SX1278.h"

status_t communication_init(void);
status_t radio_init(SX1278_hw_t *hw, SX1278_t *sx);

#endif //COMMUNICATION__H