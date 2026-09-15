/*

        * Driver written by Jakub Farnik 2026
        * Reference datasheet https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf

*/
#ifndef _BME280__H
#define _BME280__H

#include "system/status.h"
#include "libs/bosch/bme280.h"

/* Difference between BME280 and BMP280
5.2 Register compatibility to BMP280
https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf

                        BME280          |       BMP280
chip_id                 0x60                    0x56-58
0xF5 config t_sb        110,111:10,20ms         110,111:2000,4000ms

0xF7...0xF9 "press"
    BMP280: Resolution (16...20bit) depends only on osrs_p
    BME280: Without filter, resolution depends on osrs_p; 
            when using filter, resolution is always 20bit
0xFA...0xFC "temp"
    BMP280: Resolution (16...20bit) only depends on osrs_t
    BME280: Same as press in BME280 (osrs_p || 20bit)

*/

/* Register definitions */
#define REG_CHIP_ID _u(0xD0)
#define REG_RESET _u(0xE0)
    #define REG_STATUS _u(0xF3) // 2 bits measuring(0) and im_update(3y)

#define REG_CTRL_HUM _u(0xF2)
#define REG_CTRL_MEAS _u(0xF4)
#define REG_CONFIG _u(0xF5)

#define REG_PRESS_MSB _u(0xF7)
#define REG_PRESS_LSB _u(0xF8)
#define REG_PRESS_XLSB _u(0xF9)

#define REG_TEMP_MSB _u(0xFA)
#define REG_TEMP_LSB _u(0xFB)
#define REG_TEMP_XLSB _u(0xFC)

#define REG_HUM_MSB _u(0xFD)
#define REG_HUM_LSB _u(0xFE)

/*
    * For I^2C CSB (Chip select) needs to be connected to V_DDIO
    ** If pulled down, SPI interface is activated
    ** > Datasheet 6.1

    * I^2C pads contain ESD protection diodes to VDDIO and GND 6.2

    * SDO: Slave address LSB (GND = 0 ; VDDIO = 1) 6.2 Figure 8
    * SDI pull up to VDDIO                         -''-

    * SDI -> DDA ; SCK -> SCL

    * I^2C Connection diagram 7.2
    ** 100nf for C1,C2
    ** 4k7 for pull ups SDA, SCL VDDIO

*/

typedef enum {
    SENSOR_OK=0,
    COM_ERR_WRONG_DEV=10,
    TRIM_DATA_OOB=20,
    TEMP_BOND_WIRE_FAIL_MEMS_D=30,
    PRESS_BOND_WIRE_FAIL_MEMS_D=31,
    IMPLAUSIBLE_TEMP=40,
    IMPLAUSIBLE_PRESS=41,
    IMPLAUSIBLE_HUM=42
} bme280_error_t;

typedef struct{
    float pressure;
    float humidity;
    float temperature;
} bme280_t;

struct bme280_dev dev;

status_t configure_bme280(bme280_t* bme280_handler);
status_t read_bme(bme280_t* bme280_handler);


#endif // _BME280__H