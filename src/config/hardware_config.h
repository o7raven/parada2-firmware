/* Hardware Configuration */
#ifndef HARDWARE_CONFIG__H
#define HARDWARE_CONFIG__H

#define PIN_LED 25

/* UART */
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

/* I2C*/
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define I2C_BAUDRATE 400000

/* SPI */
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

#define SPI_BAUDRATE 1000000

/* RADIO */
#endif // HARDWARE_CONFIG__H


/* GPS */

#define GPS_UART uart1
#define GPS_TX 8
#define GPS_RX 9
#define GPS_BAUD 9600
