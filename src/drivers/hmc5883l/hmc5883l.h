#ifndef _HMC5883L__H_
#define _HMC5883L__H_

#include "system/status.h"

/* Register List */

#define REG_CONFIG_A 0x00
#define REG_CONFIG_B 0x01
#define REG_MODE 0x02

#define REG_DATA_X_OUT_MSB 0x03
#define REG_DATA_X_OUT_LSB 0x04

#define REG_DATA_Z_OUT_MSB 0x05
#define REG_DATA_Z_OUT_LSB 0x06

#define REG_DATA_Y_OUT_MSB 0x07
#define REG_DATA_Y_OUT_LSB 0x08

#define REG_STATUS 0x09
#define REG_IDENTIFICATION_A 0x0A
#define REG_IDENTIFICATION_B 0x0B
#define REG_IDENTIFICATION_C 0x0C


typedef struct{
    
} hmc5883l_t;

status_t configure_hmc(hmc5883l_t* hmc_handler);
status_t read_hmc(hmc5883l_t* hmc_handler);


#endif //_HMC5883L__H_