#ifndef SYSTEM__H
#define SYSTEM__H

#include "status.h"

#include "misc/blink.h"
#include "communication/communication.h"
#include "communication/logging.h"

/* @brief Initializes the system and its components
 * @return STATUS_OK if initialization was successful, otherwise returns an error status
 * @note This function should be 
 *       called before any other
 *       system functions are used and should only be called once.
 */
status_t system_init(void);

void system_run(void);

#endif //SYSTEM__H