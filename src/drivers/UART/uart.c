#include "uart.h"

status_t abstract_uart_init(void) {
    // @TODO Add error handling 

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);


    return STATUS_OK;
}