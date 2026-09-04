#include "system.h"


status_t system_init(void){

    communication_init();

    return STATUS_OK;
}