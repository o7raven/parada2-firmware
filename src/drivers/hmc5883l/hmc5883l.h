#ifndef _HMC5883L__H_
#define _HMC5883L__H_

#include "system/status.h"

/* Register List */
#define DEV_ADDR (0x1E)

#define REG_CONFIG_A (0x00)
#define REG_CONFIG_B (0x01)
#define REG_MODE (0x02)

#define REG_DATA_X_OUT_MSB (0x03)
#define REG_DATA_X_OUT_LSB (0x04)

#define REG_DATA_Z_OUT_MSB (0x05)
#define REG_DATA_Z_OUT_LSB (0x06)

#define REG_DATA_Y_OUT_MSB (0x07)
#define REG_DATA_Y_OUT_LSB (0x08)

#define REG_STATUS (0x09)

#define REG_IDENTIFICATION_A (0x0A)
#define ID_A_VAL (0b01001000)
#define REG_IDENTIFICATION_B (0x0B)
#define ID_B_VAL (0b00110100)
#define REG_IDENTIFICATION_C (0x0C)
#define ID_C_VAL (0b00110011)

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
    single = 0b01,
    continuous = 0b00,
    idle = 0b10
} mode_t;

typedef enum{
    samples_8 = 0b11,
    samples_4 = 0b10,
    samples_2 = 0b01,
    samples_1 = 0b00,
} samples_t;

typedef enum{
    range_0p88 = 0b000,
    range_1p3 = 0b001,
    range_1p9 = 0b010,
    range_2p5 = 0b011,
    range_4 = 0b100,
    range_4p7 = 0b101,
    range_5p6 = 0b110,
    range_8p1 = 0b111
} gain_settings_t;

typedef struct{
    float x_axis;
    float y_axis;
    float z_axis;
} hmc5883l_t;

status_t configure_hmc(hmc5883l_t* hmc_handler);
status_t read_hmc(hmc5883l_t* hmc_handler);

status_t set_meas_mode(mode_t mode);
status_t set_output_rate(output_rate_t rate);
status_t set_samples(samples_t samples);

hmc5883l_t self_test();



#endif //_HMC5883L__H_
