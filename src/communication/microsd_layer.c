#include "communication/logging.h"
#include "config/system_config.h"
#include "ff.h"
#include "system/status.h"
#include <string.h>
#if SD_LOGGING_ENABLED
#include "microsd_layer.h"

static FATFS fs;
static FIL file;

static bool mounted = false;
static bool recording = false;

static bool opened_once = false;

// Library definitions for the SPI interface and SD card
static spi_t spi_sd = {
    .hw_inst = SPI_PORT,

    .miso_gpio = PIN_MISO,
    .mosi_gpio = PIN_MOSI,
    .sck_gpio = PIN_SCK,

    .baud_rate = SPI_BAUDRATE,
};

static sd_spi_if_t spi_if_sd = {
    .spi = &spi_sd,
    .ss_gpio = PIN_CS,
};

static sd_card_t sd_card = {
    .type = SD_IF_SPI,
    .spi_if_p = &spi_if_sd,
};

size_t sd_get_num(void) { return 1; }

sd_card_t *sd_get_by_num(size_t num) {
  if (num == 0) {
    return &sd_card;
  }

  return NULL;
}

size_t spi_get_num(void) { return 1; }

spi_t *spi_get_by_num(size_t num) {
  if (num == 0) {
    return &spi_sd;
  }

  return NULL;
}

status_t microsd_init(void) {
  if (!sd_init_driver()) {
    return MICRO_SD_INIT;
  }
  FRESULT res = f_mount(&fs, MICROSD_DRIVE, 1);
  if (res != FR_OK) {
    return MICRO_SD_MOUNT;
  }


  res = f_mkdir(MICROSD_DRIVE "/" DATA_DIRECTORY);
  if (res != FR_OK && res != FR_EXIST) {
    return MICRO_SD_DIRECTORY_CREATION;
  }
  mounted = true;
  return STATUS_OK;
}

status_t microsd_start_recording(void) {
  if (!mounted) {
    return MICRO_SD_NOT_MOUNTED;
  }

  if (recording) {
    return MICRO_SD_ALREADY_RECORDING;
  }

  static uint32_t file_number;
  if(!opened_once){
    if (find_next_file_number(&file_number) != STATUS_OK) {
      return MICRO_SD_FILE_NUMBER_ERROR;
    }
  }

  char filename[FILE_NAME_SIZE];
  snprintf(filename, sizeof(filename), FILE_NAME_FORMAT,
           (unsigned long)file_number);
  FRESULT res = f_open(&file, filename, FA_WRITE | FA_OPEN_APPEND);
  if (res != FR_OK) {
    return MICRO_SD_FILE_OPEN;
  }
  if (!opened_once) {

    int written = f_printf(&file, "%s\r\n", CSV_NAMES);
    if (written < 0) {
      f_close(&file);
      return MICRO_SD_CSV_NAME_ERROR;
    }
  }
  opened_once = true;
  recording = true;
  return STATUS_OK;
}

status_t microsd_write(const void *data, size_t length) {
  if (!recording) {
    return MICRO_SD_NOT_RECORDING;
  }
  if (!mounted) {
    return MICRO_SD_NOT_MOUNTED;
  }

  UINT bytes_written;
  FRESULT res = f_write(&file, data, length, &bytes_written);
  if (res != FR_OK) {
    return MICRO_SD_WRITE;
  }

  if (bytes_written != length) {
    return MICRO_SD_LENGTH_MISMATCH;
  }

  return STATUS_OK;
}

status_t microsd_write_string(const char *str) {
  if (!recording) {
    return MICRO_SD_NOT_RECORDING;
  }
  if (!mounted) {
    return MICRO_SD_NOT_MOUNTED;
  }

  FRESULT res = f_puts(str, &file);
  if (res < 0) {
    return MICRO_SD_STRING_WRITE;
  }

  return STATUS_OK;
}
status_t microsd_sync(void) {
  if (!recording) {
    return MICRO_SD_NOT_RECORDING;
  }
  if (!mounted) {
    return MICRO_SD_NOT_MOUNTED;
  }

  FRESULT res = f_sync(&file);
  if (res != FR_OK) {
    return MICRO_SD_SYNC_ERROR;
  }

  return STATUS_OK;
}

status_t microsd_stop_recording(void) {
  if (!recording) {
    return MICRO_SD_NOT_RECORDING;
  }
  if (!mounted) {
    return MICRO_SD_NOT_MOUNTED;
  }

  FRESULT res = f_close(&file);
  if (res != FR_OK) {
    return MICRO_SD_FILE_CLOSE;
  }

  recording = false;
  return STATUS_OK;
}

bool microsd_is_recording(void) { return recording; }

status_t find_next_file_number(uint32_t *next_file_number) {
  FILINFO info;
  char filename[64];

  // 1000 iterations is the worst case scenario that won't ever happen.
  for (uint32_t i = 1; i < 10000; i++) {

    snprintf(filename, sizeof(filename), FILE_NAME_FORMAT, (unsigned long)i);

    FRESULT result = f_stat(filename, &info);

    if (result == FR_NO_FILE) {
      *next_file_number = i;
      return STATUS_OK;
    }

    if (result != FR_OK) {
      return MICRO_SD_FILE_STATUS_ERROR;
    }
  }

  return MICRO_SD_FILE_NUMBER_ERROR;
}

static int sync_interval = 0;
status_t write_packet(packet_t *sys_packet) {
  /*
  if (!recording) {
    return MICRO_SD_NOT_RECORDING;
  }
  if (!mounted) {
    return MICRO_SD_NOT_MOUNTED;
  }
    */
  if (sys_packet == NULL) {
    // @Note status empty packet
    log_warning("An empty packet was passed into write_packet function!");
    return MICOR_SD_EMPTY_PACKET;
  }
  if (!recording) {
    if (microsd_start_recording() != STATUS_OK) {
      log_error("MicroSD recording failed", STATUS_ERROR);
      return STATUS_ERROR;
    }
  }

  int written = f_printf(&file, CSV_FORMAT,
                         CSV_FIRST(CSV_VALUE_FIRST, sys_packet)
                             CSV_REST(CSV_VALUE_REST, sys_packet));

  if (written > 0) {
    log_info("Written %d bytes onto microSD", written);
  }
  if (sync_interval++ > 4) {
    f_sync(&file);
    sync_interval = 0;
  }
  if (microsd_stop_recording() != STATUS_OK) {
    log_error("MicroSD stop recording failed", STATUS_ERROR);
    return STATUS_ERROR;
  }
  return STATUS_OK;
}
#endif // SD_LOGGING_ENABLED