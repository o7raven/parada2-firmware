#ifndef I2C__H
#define I2C__H

#include "system/status.h"

#include "config/hardware_config.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"




status_t abstract_i2c_init(void);

status_t abstract_i2c_write(uint8_t DEV,uint8_t REG, char* BUFFER);
status_t abstract_i2c_read(uint8_t DEV, uint8_t REG, char* BUFFER);

#endif //I2C__H
