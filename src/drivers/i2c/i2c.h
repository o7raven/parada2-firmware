#ifndef I2C__H
#define I2C__H

#include "system/status.h"

#include "config/hardware_config.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

status_t abstract_i2c_init(void);

#endif //I2C__H