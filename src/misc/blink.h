#ifndef BLINK__H
#define BLINK__H
#include "system/status.h"
#include "config/hardware_config.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

static repeating_timer_t blink_timer;
static uint8_t blink_remaining;

status_t blink_init(void);
void blink(uint16_t duration, uint8_t count);
bool blink_callback(repeating_timer_t *rt);


#endif //BLINK__H