#ifndef __MICROSD_H__
#define __MICROSD_H__ 
#include "config/hardware_config.h"
#include "config/system_config.h"
#include "hw_config.h"
#include "drivers/GPS/gps.h"
#include "system/status.h"

#include "system/state_machine.h"
#include "communication/logging.h"

status_t microsd_init(void);

status_t microsd_start_recording(void);
status_t microsd_stop_recording(void);
status_t microsd_write(const void *data, size_t size);

status_t microsd_write_string(const char *str);
bool microsd_is_recording(void);

status_t find_next_file_number(uint32_t *next_file_number);

status_t microsd_sync(void);

status_t write_data_to_file(system_context_t *system_ctx, sensors_t *sensors, gps_t *gps_data);

// AI generated for saving time
#define CSV_SEPARATOR ";"
#define CSV_NAMES                                                              \
  "entry_id" CSV_SEPARATOR "radio_ok" CSV_SEPARATOR "power_ok" CSV_SEPARATOR   \
  "low_battery" CSV_SEPARATOR "critical_fault" CSV_SEPARATOR                   \
  "gps_found" CSV_SEPARATOR "radio_connected" CSV_SEPARATOR                    \
  "bme280_ok" CSV_SEPARATOR "hmc5883l_ok" CSV_SEPARATOR "imu_ok" CSV_SEPARATOR \
  "sensors_ok" CSV_SEPARATOR "bme280_pressure" CSV_SEPARATOR                   \
  "bme280_temperature" CSV_SEPARATOR "bme280_humidity" CSV_SEPARATOR           \
  "hmc5883l_X" CSV_SEPARATOR "hmc5883l_Y" CSV_SEPARATOR                        \
  "hmc5883l_Z" CSV_SEPARATOR "IMU_accX" CSV_SEPARATOR "IMU_accY" CSV_SEPARATOR \
  "IMU_accZ" CSV_SEPARATOR "IMU_gyX" CSV_SEPARATOR "IMU_gyY" CSV_SEPARATOR     \
  "IMU_gZ" CSV_SEPARATOR "lat" CSV_SEPARATOR "lon" CSV_SEPARATOR               \
  "alt" CSV_SEPARATOR "time" CSV_SEPARATOR "hdop" CSV_SEPARATOR                \
  "vdop" CSV_SEPARATOR "pdop" CSV_SEPARATOR "sats_used" CSV_SEPARATOR          \
  "sats_in_view" CSV_SEPARATOR "fix_qty" CSV_SEPARATOR "fix_type" CSV_SEPARATOR   \
  "knots"
#define CSV_FORMAT                                                             \
  "%lu" CSV_SEPARATOR "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR \
  "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR  \
  "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR "%f" CSV_SEPARATOR  \
  "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR  \
  "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR  \
  "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR  \
  "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%s" CSV_SEPARATOR "%f" CSV_SEPARATOR  \
  "%f" CSV_SEPARATOR "%f" CSV_SEPARATOR "%d" CSV_SEPARATOR "%d" CSV_SEPARATOR  \
  "%d" CSV_SEPARATOR "%f" CSV_SEPARATOR "%f"                                   \
  "\r\n"

#endif // __MICROSD_H__