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

  uint8_t register_config = (samples_1 << 5) | (rate_15Hz << 2) | 0x00;
  if (abstract_i2c_write(HMC_REG_CONFIG_A, &register_config, 1, HMC_DEV_ADDR) !=
      STATUS_OK) {
    log_warning("Issue with seting the A Config register");
    return STATUS_SENSOR_ERROR_HMC_CONFIG;
  }

  register_config = (gain_table[HMC_GAIN_SETTINGS].reg_value << 5);
  if (abstract_i2c_write(HMC_REG_CONFIG_B, &register_config, 1, HMC_DEV_ADDR) !=
      STATUS_OK) {
    log_warning("Issue with seting the B Config register");
    return STATUS_SENSOR_ERROR_HMC_CONFIG;
  }

  register_config = (mode_continuous);
  if (abstract_i2c_write(HMC_REG_MODE, &register_config, 1, HMC_DEV_ADDR) !=
      STATUS_OK) {
    log_warning("Issue with seting the Mode register");
    return STATUS_SENSOR_ERROR_HMC_CONFIG;
  }
  return STATUS_OK;
}
status_t read_hmc(hmc5883l_t* hmc_handler){

  // 0 MSB 1LSB ...
  uint8_t hmc_reg_out[6];

  abstract_i2c_read(HMC_REG_DATA_X_OUT_MSB, hmc_reg_out, 1, HMC_DEV_ADDR);

  int16_t x_out = (hmc_reg_out[0] << 8) | hmc_reg_out[1];
  int16_t z_out = (hmc_reg_out[2] << 8) | hmc_reg_out[3];
  int16_t y_out = (hmc_reg_out[4] << 8) | hmc_reg_out[5];


  hmc_handler->x_axis = convert_to_Mg(x_out);
  hmc_handler->y_axis = convert_to_Mg(z_out);
  hmc_handler->z_axis = convert_to_Mg(y_out);
  return STATUS_OK;
}

float convert_to_Mg(int16_t val){
  return val*gain_table[HMC_GAIN_SETTINGS].mg_per_digit;
}