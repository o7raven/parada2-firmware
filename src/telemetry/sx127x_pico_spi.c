/*
#include "pico/stdlib.h"

#include "hardware/spi.h"

#include "system/status.h"
#include "sx127x_spi.h"

#define RADIO_SPI spi0
#define RADIO_CS  20

static void radio_select(void) {
    gpio_put(RADIO_CS, 0);
}

static void radio_deselect(void) {
    gpio_put(RADIO_CS, 1);
}

int sx127x_spi_read_registers(
    int reg,
    void *spi_device,
    size_t data_length,
    uint32_t *result
) {
    if (!result || data_length == 0 || data_length > 4)
        return SX127X_ERR_INVALID_ARG;

    spi_inst_t *spi = (spi_inst_t *)spi_device;
    uint8_t address = (uint8_t)reg & 0x7F;
    uint8_t rx[4] = {0};

    radio_select();

    spi_write_blocking(spi, &address, 1);
    spi_read_blocking(spi, 0x00, rx, data_length);

    radio_deselect();

    *result = 0;
    for (size_t i = 0; i < data_length; i++) {
        *result = (*result << 8) | rx[i];
    }

    return SX127X_OK;
}

int sx127x_spi_read_buffer(
    int reg,
    uint8_t *buffer,
    size_t buffer_length,
    void *spi_device
) {
    if (!buffer || buffer_length == 0)
        return SX127X_ERR_INVALID_ARG;

    spi_inst_t *spi = (spi_inst_t *)spi_device;
    uint8_t address = (uint8_t)reg & 0x7F;

    radio_select();

    spi_write_blocking(spi, &address, 1);
    spi_read_blocking(spi, 0x00, buffer, buffer_length);

    radio_deselect();

    return SX127X_OK;
}

int sx127x_spi_write_register(
    int reg,
    const uint8_t *data,
    size_t data_length,
    void *spi_device
) {
    if (!data || data_length == 0 || data_length > 4)
        return SX127X_ERR_INVALID_ARG;

    spi_inst_t *spi = (spi_inst_t *)spi_device;
    uint8_t address = (uint8_t)reg | 0x80;

    radio_select();

    spi_write_blocking(spi, &address, 1);
    spi_write_blocking(spi, data, data_length);

    radio_deselect();

    return SX127X_OK;
}

int sx127x_spi_write_buffer(
    int reg,
    const uint8_t *buffer,
    size_t buffer_length,
    void *spi_device
) {
    if (!buffer || buffer_length == 0)
        return SX127X_ERR_INVALID_ARG;

    spi_inst_t *spi = (spi_inst_t *)spi_device;
    uint8_t address = (uint8_t)reg | 0x80;

    radio_select();

    spi_write_blocking(spi, &address, 1);
    spi_write_blocking(spi, buffer, buffer_length);

    radio_deselect();

    return SX127X_OK;
}

*/