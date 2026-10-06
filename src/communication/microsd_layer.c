#include "microsd_layer.h"

static FATFS fs;
static FIL file;

static bool mounted = false;
static bool recording = false;

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
    return STATUS_ERROR;
  }
  FRESULT res = f_mount(&fs,MICROSD_DRIVE ,1);
  if (res != FR_OK) {
    return STATUS_ERROR;
  }

  mounted = true;

  res = f_mkdir(MICROSD_DRIVE "/" DATA_DIRECTORY);
  if (res != FR_OK && res != FR_EXIST) {
    return STATUS_ERROR;
  }
  return STATUS_OK;
}

status_t microsd_start_recording(void) {
  if (!mounted) {
    //return not mounted error
    return STATUS_ERROR;
  }

  if (recording) {
    //return alraedy recording error
    return STATUS_ERROR;
  }

  uint32_t file_number = find_next_file_number();
  if (file_number == 0) {
    return STATUS_ERROR;
  }

  char filename[64];
  snprintf(filename, sizeof(filename), FILE_NAME_FORMAT,
           (unsigned long)file_number);
  FRESULT res = f_open(&file, filename, FA_WRITE | FA_CREATE_NEW);
  if (res != FR_OK) {
    return STATUS_ERROR;
  }

  recording = true;
  return STATUS_OK;
}

status_t microsd_write(const void *data, size_t length) {
  if (!recording || !mounted) {
    return STATUS_ERROR;
  }

  UINT bytes_written;
  FRESULT res = f_write(&file, data, length, &bytes_written);
  if (res != FR_OK || bytes_written != length) {
    return STATUS_ERROR;
  }

  if(bytes_written != length){
    return STATUS_ERROR;
  }

  return STATUS_OK;
}

status_t microsd_write_string(const char *str) {
  if (!recording || !mounted) {
    return STATUS_ERROR;
  }

  UINT bytes_written;
  FRESULT res = f_puts(str, &file);
  if (res < 0) {
    return STATUS_ERROR;
  }

  return STATUS_OK;
}
status_t microsd_sync(void){
    if (!recording || !mounted) {
        return STATUS_ERROR;
    }
    
    FRESULT res = f_sync(&file);
    if (res != FR_OK) {
        return STATUS_ERROR;
    }
    
    return STATUS_OK;
}

status_t microsd_stop_recording(void) {
  if (!recording || !mounted) {
    return STATUS_ERROR;
  }

  FRESULT res = f_close(&file);
  if (res != FR_OK) {
    return STATUS_ERROR;
  }

  recording = false;
  return STATUS_OK;
}

bool microsd_is_recording(void) { return recording; }

static uint32_t find_next_file_number(void) {
  FILINFO info;
  char filename[64];

  // only for now @NOTE change
  for (uint32_t i = 1; i < 10000; i++) {

    snprintf(filename, sizeof(filename), FILE_NAME_FORMAT, (unsigned long)i);

    FRESULT result = f_stat(filename, &info);

    if (result == FR_NO_FILE) {
      return i;
    }

    if (result != FR_OK) {
      return 0;
    }
  }

  return 0;
}