#include "system.h"


status_t system_init(void){
    blink_init();
    blink(300, 3);
    communication_init();

    return STATUS_OK;
}