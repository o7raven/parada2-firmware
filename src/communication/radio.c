#include "radio.h"
#include "communication/logging.h"
#include "system/state_machine.h"
#include "system/status.h"
#include <sys/types.h>
#include "config/hardware_config.h"
#include "telemetry/SX1278.h"
#include "telemetry/SX1278_hw.h"
#include "drivers/SPI/spi.h"
static size_t packet_size = sizeof(packet_t);
status_t send_packet(SX1278_t *sx, packet_t *sys_packet){
    static int lora_ret;


    log_info("Sending a packet of size %d bytes", packet_size);
    lora_ret = SX1278_transmit(sx, (uint8_t*)sys_packet, packet_size, 10000);
    log_info("Lora transmission: %d", lora_ret);



    return STATUS_OK;
}

status_t radio_init(SX1278_hw_t *hw, SX1278_t *sx, system_context_t* ctx){

    ctx->radio_ok = false;

    hw->dio0.pin = PIN_RADIO_DIO0;
    hw->nss.pin = PIN_RADIO_CS;
    hw->reset.pin = PIN_RADIO_RESET;
    hw->spi = SPI_PORT;

    sx->hw = hw;

    SX1278_init(sx, 43400000, SX1278_POWER_17DBM, SX1278_LORA_SF_7,
                SX1278_LORA_BW_125KHZ, SX1278_LORA_CR_4_5, SX1278_LORA_CRC_EN,
                10);

    // @Note Check if the radio is connected and working properly
    ctx->radio_ok = true;
        

    return STATUS_OK;
}