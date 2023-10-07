/*
 * adxl345.c
 *
 *  Created on: Oct 7, 2023
 *      Author: Artem
 */

#include "adxl345.h"

/*
 * @brief adxl345_bw_rate_setup - Configures output data rate
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  CS_port - CS port address
 * @param  CS_pin - CS pin number
 * @param  rate - Desired output data rate
 */
void adxl345_bw_rate_setup(SPI_HandleTypeDef* hspi, GPIO_TypeDef * CS_port, uint32_t CS_pin, uint8_t rate)
{
	uint8_t instr = BW_RATE;
	uint16_t temp;
	temp = hspi->Instance->DR;
	CS_port->ODR &= ~(1<<CS_pin);
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Transmit(hspi, &rate, 1, 100);
	CS_port->ODR |= (1<<CS_pin);
}

/*
 * @brief adxl345_measure_mode - Places the adxl345 into measurement mode
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  CS_port - CS port address
 * @param  CS_pin - CS pin number
 */
void adxl345_measure_mode(SPI_HandleTypeDef* hspi, GPIO_TypeDef * CS_port, uint32_t CS_pin)
{
	uint8_t register_data = 0;
	uint8_t instr = POWER_CTL | (1<<7);
	uint16_t temp;
	temp = hspi->Instance->DR; //Clearing buffer
	CS_port->ODR &= ~(1<<CS_pin);
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Receive(hspi, &register_data, 1, 100);
	CS_port->ODR |= (1<<CS_pin);
	register_data |= (1<<3);
	instr = POWER_CTL;
	CS_port->ODR &= ~(1<<CS_pin);
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Transmit(hspi, &register_data, 1, 100);
	CS_port->ODR |= (1<<CS_pin);
	//test
	temp = hspi->Instance->DR; //Clearing buffer
	register_data = 0;
	instr = POWER_CTL | (1<<7);
	CS_port->ODR &= ~(1<<CS_pin);
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Receive(hspi, &register_data, 1, 100);
	CS_port->ODR |= (1<<CS_pin);
}

/*
 * @brief adxl345_read_data - Gets the data from the three axes of the accelerometer.
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  CS_port - CS port address
 * @param  CS_pin - CS pin number
 * @param  buffer - Pointer to the buffer where you want to put the received data
 */
void adxl345_read_data(SPI_HandleTypeDef* hspi, GPIO_TypeDef * CS_port, uint32_t CS_pin, uint8_t* buffer)
{
	uint8_t instr = DATAX0 | (1<<7) | (1<<6);
	uint16_t temp;
	temp = hspi->Instance->DR; //Clearing buffer
	CS_port->ODR &= ~(1<<CS_pin);
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Receive(hspi, buffer, 6, 100);
	CS_port->ODR |= (1<<CS_pin);
}
