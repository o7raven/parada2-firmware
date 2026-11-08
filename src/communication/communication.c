#include "communication.h"
#include "config/hardware_config.h"
#include "system/status.h"
#include "telemetry/SX1278.h"
#include "telemetry/SX1278_hw.h"

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

status_t radio_init(SX1278_hw_t *hw, SX1278_t *sx){

    hw->dio0.pin = PIN_RADIO_DIO0;
    hw->nss.pin = PIN_RADIO_CS;
    hw->reset.pin = PIN_RADIO_RESET;
    hw->spi = SPI_PORT;

    sx->hw = hw;

    SX1278_init(sx, 43400000, SX1278_POWER_17DBM, SX1278_LORA_SF_7,
                SX1278_LORA_BW_125KHZ, SX1278_LORA_CR_4_5, SX1278_LORA_CRC_EN,
                10);
        

    return STATUS_NOT_IMPLEMENTED;
}