/**
 * Author Wojciech Domski <Wojciech.Domski@gmail.com>
 * www: www.Domski.pl
 *
 * Hardware layer for SX1278 LoRa module
 */

#include "SX1278_hw.h"
#include <pico/time.h>
#include <string.h>

#include "hardware/spi.h"
#include "hardware/gpio.h"

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0
void SX1278_hw_init(SX1278_hw_t *hw) {
  SX1278_hw_SetNSS(hw, 1);
  gpio_put(hw->reset.pin, 1);
}

void SX1278_hw_SetNSS(SX1278_hw_t *hw, int value) {
    gpio_put(hw->nss.pin, (value == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void SX1278_hw_Reset(SX1278_hw_t *hw) {
  SX1278_hw_SetNSS(hw, 1);
  gpio_put(hw->reset.pin, GPIO_PIN_RESET);

  SX1278_hw_DelayMs(1); // at least 100 us

  gpio_put(hw->reset.pin, GPIO_PIN_SET);

  SX1278_hw_DelayMs(
      10); // delay should be 5 ms minimum according to chip manual
}

void SX1278_hw_SPICommand(SX1278_hw_t *hw, uint8_t cmd) {
  SX1278_hw_SetNSS(hw, 0);
  spi_write_blocking(hw->spi, &cmd, 1);
}

uint8_t SX1278_hw_SPIReadByte(SX1278_hw_t *hw) {
  uint8_t txByte = 0x00;
  uint8_t rxByte = 0x00;

  SX1278_hw_SetNSS(hw, 0);
  spi_read_blocking(hw->spi, txByte, &rxByte, 1);
  return rxByte;
}

void SX1278_hw_DelayMs(uint32_t msec) { sleep_ms(msec); }

int SX1278_hw_GetDIO0(SX1278_hw_t *hw) {
  return (gpio_get(hw->dio0.pin) == GPIO_PIN_SET);
}
