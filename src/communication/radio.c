#include "radio.h"
#include "communication/logging.h"
#include "system/state_machine.h"
#include "system/status.h"
#include <sys/types.h>
#include "config/hardware_config.h"
#include "telemetry/SX1278.h"
#include "telemetry/SX1278_hw.h"
#include "drivers/SPI/spi.h"

#include "telemetry/sx127x_pico_spi.c"
static size_t packet_size = sizeof(packet_t);
status_t send_packet(SX1278_t *sx, packet_t *sys_packet){
    static int lora_ret;


    log_info("Sending a packet of size %d bytes", packet_size);
    //lora_ret = SX1278_transmit(sx, (uint8_t*)sys_packet, packet_size, 10000);
    char* data = "ahoj";
    lora_ret = SX1278_transmit(sx, (uint8_t*)data, 4, 3000);
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

    uint8_t version = SX1278_SPIRead(sx, 0x42);
    log_info("SX1278 version: 0x%02X", version);
    // @Note Check if the radio is connected and working properly
    if (version == 0x12) {
        log_success("SX1278 radio module is connected and working properly");
        ctx->radio_ok = true;
    } else {
        log_error("SX1278 radio module is not connected or not working properly", STATUS_RADIO_NOT_FOUND);
        ctx->radio_ok = false;
        return STATUS_RADIO_NOT_FOUND;;
    }
    SX1278_init(sx, 434000000, SX1278_POWER_17DBM, SX1278_LORA_SF_7,
                SX1278_LORA_BW_125KHZ, SX1278_LORA_CR_4_5, SX1278_LORA_CRC_EN,
                10);

        

    return STATUS_OK;
}

uint8_t test_sx1278(spi_inst_t *spi, uint cs_pin) {

    uint8_t address = 0x42;
    uint8_t version = 0;

    gpio_put(cs_pin, 0);

    spi_write_blocking(spi, &address, 1);

    spi_read_blocking(spi, 0x00, &version, 1);

    gpio_put(cs_pin, 1);

    return version;
}
void sx1278_debug_test(void) {
    gpio_put(PIN_RADIO_RESET, 0);
sleep_ms(10);

gpio_put(PIN_RADIO_RESET, 1);
sleep_ms(10);

    uint8_t address = 0x42;
    uint8_t version = 0x00;

    // Ensure SD card is deselected
   // gpio_put(PIN_CS, 1);

    // Select SX1278
    gpio_put(PIN_RADIO_CS, 0);

    log_info("NSS state: %d", gpio_get(PIN_RADIO_CS));

    spi_write_blocking(SPI_PORT, &address, 1);
    spi_read_blocking(SPI_PORT, 0x00, &version, 1);

    gpio_put(PIN_RADIO_CS, 1);

    log_info("VERSION: 0x%02X", version);
}

