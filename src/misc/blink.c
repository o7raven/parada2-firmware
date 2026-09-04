#include "blink.h"

static uint8_t blink_remaining = 0;

status_t blink_init(void){
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED, 0);
    return STATUS_OK;
}

void blink(uint16_t duration, uint8_t count){
   blink_remaining = count * 2;
   add_repeating_timer_ms(
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
    } else {
        gpio_put(PIN_LED, 0);
        return false;
    }
}