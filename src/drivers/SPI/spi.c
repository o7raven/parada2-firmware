#include "spi.h"
#include "communication/logging.h"

status_t abstract_spi_init(void)
{
    // Initialize SPI1
    spi_init(SPI_PORT, SPI_BAUDRATE);

    spi_set_format(
        SPI_PORT,
        8,
        SPI_CPOL_0,
        SPI_CPHA_0,
        SPI_MSB_FIRST
    );

    // SPI pins
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);



     return STATUS_OK;
}
