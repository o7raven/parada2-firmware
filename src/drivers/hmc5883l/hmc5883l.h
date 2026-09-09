#ifndef _HMC5883L__H_
#define _HMC5883L__H_

#include "system/status.h"

typedef struct{
    
} hmc5883l_t;

status_t configure_hmc(hmc5883l_t* hmc_handler);
status_t read_hmc(hmc5883l_t* hmc_handler);


#endif //_HMC5883L__H_