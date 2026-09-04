#include "communication.h"

status_t communication_init(void){
    if(abstract_uart_init() != STATUS_OK){
        return STATUS_UART_ERROR;
    }
    if(abstract_i2c_init() != STATUS_OK){
        return STATUS_I2C_ERROR;
    }
    if(abstract_spi_init() != STATUS_OK){
        return STATUS_SPI_ERROR;
    }
    return STATUS_OK;
}