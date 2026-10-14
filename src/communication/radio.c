#include "radio.h"
#include "communication/logging.h"
#include "system/state_machine.h"
#include "system/status.h"
#include <sys/types.h>

status_t init_radio(system_context_t* ctx){
    ctx->radio_ok = false;
    ctx->radio_connected = false;

    return STATUS_NOT_IMPLEMENTED;
}
