#include "i2c.h"
#include "communication/logging.h"
#include "system/status.h"
#include <hardware/i2c.h>
#include <stdbool.h>
#include <stdint.h>

status_t abstract_i2c_init(void) {
    // @TODO Add error handling 
    i2c_init(I2C_PORT, I2C_BAUDRATE);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    return STATUS_OK;
}

status_t abstract_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                            uint32_t length, uint8_t dev_addr) {
  uint8_t buffer_to_write[length + 1];
  buffer_to_write[0] = reg_addr;
  for (uint32_t i = 0; i < length; i++) {
    buffer_to_write[i + 1] = reg_data[i];
  }
  int result = i2c_write_blocking(I2C_PORT, dev_addr, buffer_to_write,
                                  length + 1, false);
  if (result < 0) {
    log_error("Error in i2c write", STATUS_ERROR);
    return STATUS_ERROR;
  }
  return STATUS_OK;
}

status_t abstract_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length,
                           uint8_t dev_addr) {
  i2c_write_blocking(I2C_PORT, dev_addr, &reg_addr, 1, true);
  i2c_read_blocking(I2C_PORT, dev_addr, reg_data, length, false);
  return STATUS_OK;
}