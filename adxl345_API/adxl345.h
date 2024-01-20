/*
 * adxl345.h
 *
 *  Created on: Oct 7, 2023
 *      Author: Artem
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "main.h"

#define DEVID 0x0
#define BW_RATE 0x2C
#define POWER_CTL 0x2D
#define INT_ENABLE 0x2E
#define INT_MAP 0x2F
#define INT_SOURCE 0x30
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

void adxl345_bw_rate_setup(SPI_HandleTypeDef* hspi, GPIO_TypeDef * CS_port, uint32_t CS_pin, uint8_t rate);
void adxl345_measure_mode(SPI_HandleTypeDef* hspi, GPIO_TypeDef * CS_port, uint32_t CS_pin);
void adxl345_data_format(SPI_HandleTypeDef* hspi, GPIO_TypeDef * CS_port, uint32_t CS_pin, uint8_t data_format);
void adxl345_read_data(SPI_HandleTypeDef* hspi, GPIO_TypeDef * CS_port, uint32_t CS_pin, uint8_t* buffer);

#endif /* INC_ADXL345_H_ */
