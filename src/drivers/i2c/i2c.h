#ifndef I2C__H
#define I2C__H

#include "system/status.h"

#include "config/hardware_config.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"




status_t abstract_i2c_init(void);

status_t abstract_i2c_write(uint8_t reg_addr, const uint8_t* reg_data,
                            uint32_t length, uint8_t dev_addr);
status_t abstract_i2c_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length, uint8_t dev_addr);

#endif //I2C__H
