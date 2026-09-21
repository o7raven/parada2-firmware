#ifndef _HMC5883L__H_
#define _HMC5883L__H_

#include "system/status.h"
#include "inttypes.h"
#include <stdint.h>

/* Register List */
#define HMC_DEV_ADDR (0x1E)

#define HMC_REG_CONFIG_A (0x00)
#define HMC_REG_CONFIG_B (0x01)
#define HMC_REG_MODE (0x02)

#define HMC_REG_DATA_X_OUT_MSB (0x03)
#define HMC_REG_DATA_X_OUT_LSB (0x04)

#define HMC_REG_DATA_Z_OUT_MSB (0x05)
#define HMC_REG_DATA_Z_OUT_LSB (0x06)

#define HMC_REG_DATA_Y_OUT_MSB (0x07)
#define HMC_REG_DATA_Y_OUT_LSB (0x08)

#define HMC_REG_STATUS (0x09)

#define HMC_REG_IDENTIFICATION_A (0x0A)
#define HMC_ID_A_VAL (0b01001000)
#define HMC_REG_IDENTIFICATION_B (0x0B)
#define HMC_ID_B_VAL (0b00110100)
#define HMC_REG_IDENTIFICATION_C (0x0C)
#define HMC_ID_C_VAL (0b00110011)

#define OVERFLOW_VALUE -4096

// Inspired https://github.com/jarzebski/Arduino-HMC5883L/blob/dev/HMC5883L.h
typedef enum{
    rate_0p75Hz = 0b000,
    rate_1p5Hz = 0b001,
    rate_3Hz = 0b010,
    rate_7p5Hz = 0b011,
    rate_15Hz = 0b100,
    rate_30Hz = 0b101,
    rate_75Hz = 0b110,
} output_rate_t;

typedef enum{
    mode_single = 0b01,
    mode_continuous = 0b00,
    mode_idle = 0b10
} mode_type_t;

typedef enum{
    samples_8 = 0b11,
    samples_4 = 0b10,
    samples_2 = 0b01,
    samples_1 = 0b00,
} samples_t;

typedef struct{
    uint8_t reg_value;
    float mg_per_digit;
} hmc_gain_config_t;

typedef enum {
  range_0p88 = 0,
  range_1p3,
  range_1p9,
  range_2p5,
  range_4,
  range_4p7, 
  range_5p6,
  range_8p1
} gain_table_t;
static const hmc_gain_config_t gain_table[8] = {
    {0b000, 0.73f}, // 0.88Ga
    {0b001, 0.92f}, // 1.3Ga
    {0b010, 1.22f}, // 1.9Ga
    {0b011, 1.52f}, // 2.g Ga
    {0b100, 2.27f}, // 4.0 Ga
    {0b101, 2.56f}, // 4.7 Ga
    {0b110, 3.03f}, //5.6 Ga
    {0b111, 4.35f}, //8.1 Ga
};

typedef struct{
    float x_axis;
    float y_axis;
    float z_axis;
} hmc5883l_t;

status_t configure_hmc(hmc5883l_t* hmc_handler);
status_t read_hmc(hmc5883l_t* hmc_handler);

status_t set_meas_mode(mode_type_t mode);
status_t set_output_rate(output_rate_t rate);
status_t set_samples(samples_t samples);
status_t set_range(gain_table_t _gT);

hmc5883l_t self_test();

float convert_to_Mg(int16_t val);



#endif //_HMC5883L__H_
