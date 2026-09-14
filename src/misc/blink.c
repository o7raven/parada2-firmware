#include "blink.h"

static uint8_t blink_remaining = 0;
static repeating_timer_t blink_timer;
static bool blink_timer_running = false;

status_t blink_init(void) {
  gpio_init(PIN_LED);
  gpio_set_dir(PIN_LED, GPIO_OUT);
  gpio_put(PIN_LED, 0);

  blink_timer_running = false;

  return STATUS_OK;
}

void blink(uint16_t duration, uint8_t count){
    if(blink_timer_running){
        cancel_repeating_timer(&blink_timer);
        blink_timer_running = false;
    }
   blink_remaining = count * 2;
   blink_timer_running = add_repeating_timer_ms(
    duration,
    blink_callback,
    NULL,
    &blink_timer
   ); 
}

bool blink_callback(repeating_timer_t *rt){
    if(blink_remaining > 0){
        gpio_xor_mask(1 << PIN_LED);
        blink_remaining--;
        return true;
    }
    blink_timer_running = false;
    gpio_put(PIN_LED, 0);
    return false;
}