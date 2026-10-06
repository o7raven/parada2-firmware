#ifndef __MICROSD_H__
#define __MICROSD_H__ 
#include "config/hardware_config.h"
#include "hw_config.h"
#include "system/status.h"

status_t microsd_init(void);

status_t microsd_start_recording(void);
status_t microsd_stop_recording(void);
status_t microsd_write(const void *data, size_t size);

status_t microsd_write_string(const char *str);
bool microsd_is_recording(void);

static uint32_t find_next_file_number(void);

status_t microsd_sync(void);





#endif // __MICROSD_H__