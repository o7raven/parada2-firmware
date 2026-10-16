#ifndef __MICROSD_H__
#define __MICROSD_H__

#include "config/system_config.h"
#if SD_LOGGING_ENABLED
#include "config/hardware_config.h"
#include "drivers/GPS/gps.h"
#include "hw_config.h"
#include "system/status.h"

#include "communication/logging.h"
#include "system/state_machine.h"
#include "telemetry/packet.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CSV_SEPARATOR ";"

/*
 * First field: no comma before its value.
 */
#define CSV_FIRST(X, p) \
    X("packet_id", "%lu", (unsigned long)(p)->packet_id, CSV_SEPARATOR)
/*
 * Remaining fields: each generated value starts with a comma.
 */
#define CSV_REST(X, p) \
    X("state_current_state",  "%d", (int)(p)->state_m.current_state, CSV_SEPARATOR) \
    X("state_previous_state", "%d", (int)(p)->state_m.previous_state, CSV_SEPARATOR) \
    X("state_time_ms", "%lu", (unsigned long)(p)->state_m.state_time_ms, CSV_SEPARATOR) \
    X("radio_ok", "%u", (unsigned)(p)->system_ctx.radio_ok, CSV_SEPARATOR) \
    X("power_ok", "%u", (unsigned)(p)->system_ctx.power_ok, CSV_SEPARATOR) \
    X("low_battery", "%u", (unsigned)(p)->system_ctx.low_battery, CSV_SEPARATOR) \
    X("critical_fault", "%u", (unsigned)(p)->system_ctx.critical_fault, CSV_SEPARATOR) \
    X("gps_found", "%u", (unsigned)(p)->system_ctx.gps_found, CSV_SEPARATOR) \
    X("radio_connected", "%u", (unsigned)(p)->system_ctx.radio_connected, CSV_SEPARATOR) \
    X("bme280_ok", "%u", (unsigned)(p)->system_ctx.sensors.bme280_ok, CSV_SEPARATOR) \
    X("hmc5883l_ok", "%u", (unsigned)(p)->system_ctx.sensors.hmc5883l_ok, CSV_SEPARATOR) \
    X("imu_ok", "%u", (unsigned)(p)->system_ctx.sensors.imu_ok, CSV_SEPARATOR) \
    X("sensors_ok", "%u", (unsigned)(p)->system_ctx.sensors.sensors_ok, CSV_SEPARATOR) \
    X("bme280_pressure", "%f", (double)(p)->sensors.bme280.pressure, CSV_SEPARATOR) \
    X("bme280_temperature", "%f", (double)(p)->sensors.bme280.temperature, CSV_SEPARATOR) \
    X("bme280_humidity", "%f", (double)(p)->sensors.bme280.humidity, CSV_SEPARATOR) \
    X("hmc5883l_x", "%f", (double)(p)->sensors.hmc.x_axis, CSV_SEPARATOR) \
    X("hmc5883l_y", "%f", (double)(p)->sensors.hmc.y_axis, CSV_SEPARATOR) \
    X("hmc5883l_z", "%f", (double)(p)->sensors.hmc.z_axis, CSV_SEPARATOR) \
    X("imu_acc_x", "%f", (double)(p)->sensors.imu.acc_X, CSV_SEPARATOR) \
    X("imu_acc_y", "%f", (double)(p)->sensors.imu.acc_Y, CSV_SEPARATOR) \
    X("imu_acc_z", "%f", (double)(p)->sensors.imu.acc_Z, CSV_SEPARATOR) \
    X("imu_gyro_x", "%f", (double)(p)->sensors.imu.gyro_X, CSV_SEPARATOR) \
    X("imu_gyro_y", "%f", (double)(p)->sensors.imu.gyro_Y, CSV_SEPARATOR) \
    X("imu_gyro_z", "%f", (double)(p)->sensors.imu.gyro_Z, CSV_SEPARATOR) \
    X("gps_valid", "%u", (unsigned)(p)->gps_data.valid, CSV_SEPARATOR) \
    X("gps_lon", "%f", (double)(p)->gps_data.lon, CSV_SEPARATOR) \
    X("gps_lat", "%f", (double)(p)->gps_data.lat, CSV_SEPARATOR) \
    X("gps_alt", "%f", (double)(p)->gps_data.alt, CSV_SEPARATOR) \
    X("gps_time", "%s", (p)->gps_data.time, CSV_SEPARATOR) \
    X("gps_hdop", "%f", (double)(p)->gps_data.hdop, CSV_SEPARATOR) \
    X("gps_vdop", "%f", (double)(p)->gps_data.vdop, CSV_SEPARATOR) \
    X("gps_pdop", "%f", (double)(p)->gps_data.pdop, CSV_SEPARATOR) \
    X("gps_sats_used", "%u", (unsigned)(p)->gps_data.sats_used, CSV_SEPARATOR) \
    X("gps_sats_in_view", "%u", (unsigned)(p)->gps_data.sats_in_view, CSV_SEPARATOR) \
    X("gps_fix_qty", "%u", (unsigned)(p)->gps_data.fix_qty, CSV_SEPARATOR) \
    X("gps_fix_type", "%u", (unsigned)(p)->gps_data.fix_type, CSV_SEPARATOR) \
    X("gps_knots", "%f", (double)(p)->gps_data.knots, CSV_SEPARATOR) \
    X("crc16", "%u", (unsigned)(p)->crc16, "")

/* Generate column names. */
#define CSV_HEADER_FIELD(name, fmt, value, sep) name sep

#define CSV_NAMES \
    CSV_FIRST(CSV_HEADER_FIELD, p) \
    CSV_REST(CSV_HEADER_FIELD, p)

/* Generate printf format specifiers. */
#define CSV_FORMAT_FIELD(name, fmt, value, sep) fmt sep

#define CSV_FORMAT \
    CSV_FIRST(CSV_FORMAT_FIELD, p) \
    CSV_REST(CSV_FORMAT_FIELD, p) "\r\n"

/* First value has no leading comma. */
#define CSV_VALUE_FIRST(name, fmt, value, sep) value

/* Every subsequent value has a leading comma. */
#define CSV_VALUE_REST(name, fmt, value, sep) , value

status_t microsd_init(void);

status_t microsd_start_recording(void);
status_t microsd_stop_recording(void);
status_t microsd_write(const void *data, size_t size);

status_t microsd_write_string(const char *str);
bool microsd_is_recording(void);

status_t find_next_file_number(uint32_t *next_file_number);

status_t microsd_sync(void);

status_t write_packet(packet_t *sys_packet);

#endif // SD_LOGGING_ENABLED
#endif // __MICROSD_H__
