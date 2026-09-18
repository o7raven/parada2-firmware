#include "hmc5883l.h"
#include "communication/logging.h"
#include "drivers/i2c/i2c.h"
#include "system/status.h"

status_t configure_hmc(hmc5883l_t *hmc_handler) {
  // Check the Identification registers
  log_info("Accessing HMC5883l identification registers...");
  uint8_t sensor_identification[3];
  if (abstract_i2c_read(HMC_REG_IDENTIFICATION_A, &sensor_identification[0], 1,
                        HMC_DEV_ADDR) != STATUS_OK) {
    log_warning(
        "There has been an error reading the Identification A register");
    return STATUS_SENSOR_INIT_FAIL;
  }
  if (abstract_i2c_read(HMC_REG_IDENTIFICATION_B, &sensor_identification[1], 1,
                        HMC_DEV_ADDR) != STATUS_OK) {
    log_warning(
        "There has been an error reading the Identification B register");
    return STATUS_SENSOR_INIT_FAIL;
  }
  if (abstract_i2c_read(HMC_REG_IDENTIFICATION_C, &sensor_identification[2], 1,
                        HMC_DEV_ADDR) != STATUS_OK) {
    log_warning(
        "There has been an error reading the Identification C register");
    return STATUS_SENSOR_INIT_FAIL;
  }
  if (sensor_identification[0] == HMC_ID_A_VAL &&
      sensor_identification[1] == HMC_ID_B_VAL &&
      sensor_identification[2] == HMC_ID_C_VAL) {
    log_success("HMC5883l has been verified!");
  } else {
    log_warning("HMC5883l has not been verified");
    return STATUS_SENSOR_INIT_FAIL;
  }

  uint8_t register_config = (0b0110000) | (rate_15Hz << 2) | (samples_1);
  if (abstract_i2c_write(HMC_REG_CONFIG_A, &register_config, 1, HMC_DEV_ADDR) !=
      STATUS_OK) {
    log_warning("Issue with seting the A Config register");
    return STATUS_SENSOR_ERROR_HMC_CONFIG;
  }

  register_config = (0b11100000) & (range_1p3 << 5);
  if (abstract_i2c_write(HMC_REG_CONFIG_B, &register_config, 1, HMC_DEV_ADDR) !=
      STATUS_OK) {
    log_warning("Issue with seting the B Config register");
    return STATUS_SENSOR_ERROR_HMC_CONFIG;
  }

  register_config = (0b00000011) & (mode_continuous);
  if (abstract_i2c_write(HMC_REG_MODE, &register_config, 1, HMC_DEV_ADDR) !=
      STATUS_OK) {
    log_warning("Issue with seting the Mode register");
    return STATUS_SENSOR_ERROR_HMC_CONFIG;
  }
  return STATUS_OK;
}
status_t read_hmc(hmc5883l_t* hmc_handler){
  uint8_t x_reg_MSB;
  uint8_t x_reg_LSB;

  abstract_i2c_read(HMC_REG_DATA_X_OUT_MSB, &x_reg_MSB, 1, HMC_DEV_ADDR);
  abstract_i2c_read(HMC_REG_DATA_X_OUT_LSB, &x_reg_LSB, 1, HMC_DEV_ADDR);

  int16_t x_out = (x_reg_MSB << 8) | x_reg_LSB;

  uint8_t y_reg_MSB;
  uint8_t y_reg_LSB;

  abstract_i2c_read(HMC_REG_DATA_Y_OUT_MSB, &y_reg_MSB, 1, HMC_DEV_ADDR);
  abstract_i2c_read(HMC_REG_DATA_Y_OUT_LSB, &y_reg_LSB, 1, HMC_DEV_ADDR);

  int16_t y_out = (y_reg_MSB << 8) | y_reg_LSB;

  uint8_t z_reg_MSB;
  uint8_t z_reg_LSB;

  abstract_i2c_read(HMC_REG_DATA_Z_OUT_MSB, &z_reg_MSB, 1, HMC_DEV_ADDR);
  abstract_i2c_read(HMC_REG_DATA_Z_OUT_LSB, &z_reg_LSB, 1, HMC_DEV_ADDR);

  int16_t z_out = (z_reg_MSB << 8) | z_reg_LSB;

  hmc_handler->x_axis = (float)x_out;
  hmc_handler->y_axis = (float)y_out;
  hmc_handler->z_axis = (float)z_out;
  return STATUS_OK;
}