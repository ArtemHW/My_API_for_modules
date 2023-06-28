/*
 * w25q64fv.c
 *
 *  Created on: 27.06.2023.
 *      Author: Artem Kagirov
 */

#include "w25q64fv.h"


/*
 * @brief  write_enable - The Write Enable instruction sets the Write Enable Latch (WEL)
 *         bit in the Status Register to a 1. The WEL bit must be set prior to
 *         every Page Program, Quad Page Program, Sector Erase, Block Erase, Chip Erase,
 *         Write Status Register and Erase/Program Security Registers instruction.
 * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32 family
 * @param  GPIO_Pin_CS - pin on your MCU that is connected to CS pin W25Q64FV
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 */
void write_enable(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, SPI_HandleTypeDef * hspi)
{
	uint8_t result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	while(result != 0)
	{
		result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	}
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = WRITE_ENABLE;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
}

/*
 * @brief  write_disable - The Write Disable instruction resets the Write Enable Latch (WEL)
 * 		    bit in the Status Register to a 0.
 * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32 family
 * @param  GPIO_Pin_CS - pin on your MCU that is connected to CS pin W25Q64FV
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 */
void write_disable(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, SPI_HandleTypeDef * hspi)
{
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = WRITE_DISABLE;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
}

/*
 * @brief  check_if_busy - BUSY is a read only bit in the status register (S0)
 *         that is set to a 1 state when the device is executing a Page Program,
 *         Quad Page Program, Sector Erase, Block Erase, Chip Erase, Write Status
 *         Register or Erase/Program Security Register instruction.
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *         the configuration information for SPI module.
 * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32 family
 * @param  GPIO_Pin_CS - pin on your MCU that is connected to CS pin W25Q64FV
 * @retval result - if result is 0 then BUSY bit = 0, else if result is 1 then BUSY bit = 1,
 *         else result is 3.
 */
uint8_t check_if_busy(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS)
{
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = READ_STATUS_REG1;
	uint8_t result;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Receive(hspi, &result, 1, 100);
	result &= (1<<0);
	GPIOx->ODR |= GPIO_Pin_CS;
	if(result != 1 && result != 0)
	{
		result = 3;
	}
	return result;
}

void read_status_registers(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint8_t* answer)
{
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = READ_STATUS_REG1;
	uint8_t result;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Receive(hspi, &result, 1, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	instr = READ_STATUS_REG2;
	uint8_t result2;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	HAL_SPI_Receive(hspi, &result2, 1, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
	*answer = result;
	*(answer+1) = result2;
}

/*
 * @brief  sector_erase - Sector erase (4KB)
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *         the configuration information for SPI module.
 * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32 family
 * @param  GPIO_Pin_CS - pin on your MCU that is connected to CS pin W25Q64FV
 * @param  address - 24-bit address
 */
void sector_erase(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint32_t address)
{
	uint8_t result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	while(result != 0)
	{
		result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	}
	write_enable(GPIOx, GPIO_Pin_CS, hspi);
	do
	{
		result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	}
	while(result != 0);
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = SECTOR_ERASE;
	uint8_t TxData[4];
	memcpy(TxData, &instr, 1);
	uint8_t Arr_address[3] = {*(((uint8_t*)(&address))+2), *(((uint8_t*)(&address))+1), *(((uint8_t*)(&address)))};
	memcpy(TxData+1, Arr_address, 3);
	HAL_SPI_Transmit(hspi, TxData, 4, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
}

/*
 * @brief  chip_erase - Full chip erase
 * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32 family
 * @param  GPIO_Pin_CS - pin on your MCU that is connected to CS pin W25Q64FV
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 */
void chip_erase(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, SPI_HandleTypeDef * hspi)
{
	write_enable(GPIOx, GPIO_Pin_CS, hspi);
	uint8_t result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	while(result != 0)
	{
		result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	}
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = CHIP_ERASE;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
}

/*
 * @brief  page_program - Programs the page (one page has 256 bytes)
 * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains the
 * 		   configuration information for SPI module.
 * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32 family
 * @param  GPIO_Pin_CS - pin on your MCU that is connected to CS pin W25Q64FV
 * @param  address - 24-bit address
 * @param  pData: pointer to data buffer
 * @param  Size: amount of data to be sent
 */
void page_program(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint32_t address, uint8_t * pData, uint16_t Size)
{
	write_enable(GPIOx, GPIO_Pin_CS, hspi);
	uint8_t result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	while(result != 0)
	{
		result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	}
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = PAGE_PROGRAM;
//	uint16_t fullSize = sizeof(instr) + sizeof(address) + Size;
	uint16_t fullSize = 4 + Size;
	uint8_t Data[fullSize];
	memcpy(Data, &instr, 1);
	uint8_t Arr_address[3] = {*(((uint8_t*)(&address))+2), *(((uint8_t*)(&address))+1), *(((uint8_t*)(&address)))};
	memcpy(Data+1, Arr_address, 3);
	memcpy(Data+4, pData, Size);
	HAL_SPI_Transmit(hspi, Data, fullSize, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
}

/*
 * @brief  read_data - The Read Data instruction allows one or more data bytes
 *         to be sequentially read from the memory
 * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains the
 * 		   configuration information for SPI module.
 * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32 family
 * @param  GPIO_Pin_CS - pin on your MCU that is connected to CS pin W25Q64FV
 * @param  address - 24-bit address
 * @param  pData: pointer to data buffer
 * @param  Size: amount of data to be received
 */
void read_data(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS, uint32_t address, uint8_t * pData, uint16_t Size)
{
	uint8_t result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	while(result != 0)
	{
		result = check_if_busy(hspi, GPIOx, GPIO_Pin_CS);
	}
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = READ_DATA;
	uint8_t TxData[4];
	memcpy(TxData, &instr, 1);
	uint8_t Arr_address[3] = {*(((uint8_t*)(&address))+2), *(((uint8_t*)(&address))+1), *(((uint8_t*)(&address)))};
	memcpy(TxData+1, Arr_address, 3);
	HAL_SPI_Transmit(hspi, TxData, 4, 100);
	HAL_SPI_Receive(hspi, pData, Size, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
}

void reset_generation(SPI_HandleTypeDef * hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS)
{
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	uint8_t instr = ENABLE_RESET;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
	GPIOx->ODR &= ~GPIO_Pin_CS; // CS
	instr = RESET;
	HAL_SPI_Transmit(hspi, &instr, 1, 100);
	GPIOx->ODR |= GPIO_Pin_CS;
}
