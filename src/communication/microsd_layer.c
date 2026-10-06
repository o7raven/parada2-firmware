#include "microsd_layer.h"

static FATFS fs;
static FIL file;

static bool mounted = false;
static bool recording = false;

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

  uint32_t file_number;
  if (find_next_file_number(&file_number) != STATUS_OK) {
    return MICRO_SD_FILE_NUMBER_ERROR;
  }

  char filename[FILE_NAME_SIZE];
  snprintf(filename, sizeof(filename), FILE_NAME_FORMAT,
           (unsigned long)file_number);
  FRESULT res = f_open(&file, filename, FA_WRITE | FA_CREATE_NEW);
  if (res != FR_OK) {
    return MICRO_SD_FILE_OPEN;
  }

  int written = f_printf(&file, "%s\r\n", CSV_NAMES);
  if (written < 0) {
    f_close(&file);
    return MICRO_SD_CSV_NAME_ERROR;
  }
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

status_t write_data_to_file(system_context_t *system_ctx, sensors_t *sensors,
                            gps_t *gps_data) {
  if (!recording) {
    return MICRO_SD_NOT_RECORDING;
  }
  if (!mounted) {
    return MICRO_SD_NOT_MOUNTED;
  }

  static uint32_t id = 1;

   int written = f_printf(
      &file, CSV_FORMAT, (unsigned long)id++, system_ctx->radio_ok, system_ctx->power_ok,
      system_ctx->low_battery, system_ctx->critical_fault,
      system_ctx->gps_found, system_ctx->radio_connected,
      system_ctx->sensors.bme280_ok, system_ctx->sensors.hmc5883l_ok,
      system_ctx->sensors.imu_ok, system_ctx->sensors.sensors_ok,
      sensors->bme280.pressure, sensors->bme280.temperature,
      sensors->bme280.humidity, sensors->hmc.x_axis, sensors->hmc.y_axis,
      sensors->hmc.z_axis, sensors->imu.acc_X, sensors->imu.acc_Y,
      sensors->imu.acc_Z, sensors->imu.gyro_X, sensors->imu.gyro_Y,
      sensors->imu.gyro_Z, gps_data->lat, gps_data->lon, gps_data->alt,
      gps_data->time, gps_data->hdop, gps_data->vdop, gps_data->pdop,
      gps_data->sats_used, gps_data->sats_in_view, gps_data->fix_qty,
      gps_data->fix_type, gps_data->knots);
  if (written < 0) {
    return MICRO_SD_WRITE;
  }
  log_info("Data written to MicroSD: %d bytes", written);
  return STATUS_OK;
}