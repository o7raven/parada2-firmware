/* System Configuration */
#ifndef SYSTEM_CONFIG__H
#define SYSTEM_CONFIG__H

// System Configuration
#define FIRMWARE_VERSION "0.1"
#define WATCHDOG_TIMEOUT_ms 3000
#define WATCHDOG_PAUSE_ON_DBG 1
#define BOOT_TIME_TO_INIT_ms 100

#define LOG_BLINK_ms 50


// Radio Configuration
#define DATA_COLLECTION_PERIOD_ms 1500
#define RUN_STATE_PACKET_FREQUENCY_ms 5000
#define SAFE_STATE_PACKET_FREQUENCY_ms 15000 

// GPS Configuration



// MicroSD Configuration
#define FILE_NAME_SIZE 32

#endif // SYSTEM_CONFIG__H