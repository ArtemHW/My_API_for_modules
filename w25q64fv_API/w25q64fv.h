/*
 * w25q64fv.h
 *
 *  Created on: 27.06.2023.
 *      Author: Artem Kagirov
 */

#ifndef INC_W25Q64FV_H_
#define INC_W25Q64FV_H_

#include "main.h"

/* Instructions for W25Q64FV chip */
#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define READ_STATUS_REG1 0x05
#define READ_STATUS_REG2 0x35
#define PAGE_PROGRAM 0x02
#define READ_DATA 0x03
#define SECTOR_ERASE 0x20
#define CHIP_ERASE 0xC7
#define ENABLE_RESET 0x66
#define RESET 0x99

void write_enable(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, SPI_HandleTypeDef * hspi);
void write_disable(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, SPI_HandleTypeDef * hspi);
uint8_t check_if_busy(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS);
void read_status_registers(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint8_t* answer);
void sector_erase(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint32_t address);
void chip_erase(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, SPI_HandleTypeDef * hspi);
void page_program(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint32_t address, uint8_t * pData, uint16_t Size);
void read_data(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint32_t address, uint8_t * pData, uint16_t Size);
void reset_generation(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS);

#endif /* INC_W25Q64FV_H_ */
