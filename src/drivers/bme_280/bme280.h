/*

        * Driver written by Jakub Farnik 2026
        * Reference datasheet https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf

*/
#ifndef _BME280__H
#define _BME280__H

#include "system/status.h"

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
#define REG_CHIP_ID 0xD0
#define REG_RESET 0xE0
    #define REG_STATUS 0xF3 // 2 bits measuring(0) and im_update(3y)

#define REG_CTRL_HUM 0xF2
#define REG_CTRL_MEAS 0xF4
#define REG_CONFIG 0xF5


typedef struct{
    float pressure;
    float humidity;
    float temperature;
} bme280_t;

status_t configure_bme280(bme280_t* bme280_handler);
status_t read_bme(bme280_t* bme280_handler);


#endif // _BME280__H