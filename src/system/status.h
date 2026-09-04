#ifndef STATUS__H
#define STATUS__H

typedef enum {
  STATUS_OK = 0,
  STATUS_ERROR,
  STATUS_TIMEOUT,
  STATUS_NOT_FOUND,
  STATUS_INVALID_PARAM,
  STATUS_NOT_INITIALIZED,
  STATUS_I2C_ERROR,
  STATUS_SPI_ERROR,
  STATUS_UART_ERROR,
  STATUS_SENSOR_ERROR,
  STATUS_SENSOR_NOT_FOUND
} status_t;
// Expand the status_t typedef and split different modules into their own enums;
#endif //STATUS__H