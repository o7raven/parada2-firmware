#ifndef SPI__H
#define SPI__H

#include "system/status.h"
#include "config/hardware_config.h"

#include "hardware/spi.h"
#include "hardware/gpio.h"

status_t abstract_spi_init(void);

#endif //SPI__H