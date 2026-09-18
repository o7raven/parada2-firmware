#include "hmc5883l.h"
#include "communication/logging.h"
#include "drivers/i2c/i2c.h"
#include "system/status.h"

status_t configure_hmc(hmc5883l_t* hmc_handler){
    // Check the Identification registers
    log_info("Accessing HMC5883l identification registers...");
    uint8_t sensor_identification[3];
    if(abstract_i2c_read(REG_IDENTIFICATION_A, &sensor_identification[0], 1, DEV_ADDR) != STATUS_OK){
        log_warning("There has been an error reading the Identification A register");
    }
    if(abstract_i2c_read(REG_IDENTIFICATION_B, &sensor_identification[1], 1, DEV_ADDR) != STATUS_OK){
        log_warning("There has been an error reading the Identification B register");
    }
    if(abstract_i2c_read(REG_IDENTIFICATION_C, &sensor_identification[2], 1, DEV_ADDR) != STATUS_OK){
        log_warning("There has been an error reading the Identification C register");
    }
    if(sensor_identification[0] == ID_A_VAL && sensor_identification[1] == ID_B_VAL && sensor_identification[2] == ID_C_VAL){
        log_success("HMC5883l has been verified!");
    }else{
        log_warning("HMC5883l has not been verified");
    }



    return STATUS_NOT_IMPLEMENTED;
}
status_t read_hmc(hmc5883l_t* hmc_handler){
    return STATUS_NOT_IMPLEMENTED;
}