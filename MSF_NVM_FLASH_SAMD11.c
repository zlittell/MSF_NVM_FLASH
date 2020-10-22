/**
* @file MSF_NVM_FLASH_SAMD11.c
* @brief FLASH R/W Library for SAMD11 internal Flash
* @author Zack Littell
* @company Mechanical Squid Factory
* @project MSF_NVM_FLASH Library
*/

#include <sam.h>

/**
	@brief Read Page
	@details Reads a page from flash at specified memory address
	@param Memory address to start read at
	@param Pointer to array to store read data in
*/
void flash_page_read(uint16_t memAddr, uint32_t *readData)
{
	for(uint8_t i = 0; i < 16; i++)
	{
		*(readData + i) = *((uint32_t *)((uint32_t)memAddr) + i);
	}
}

/**
	@brief Write Verify
	@details Verifies page of memory against array passed.
	@param Memory address to start verify at
	@param Data to compare memory to
	@returns Success
*/
uint8_t flash_page_write_verify(uint16_t memAddr, uint32_t *dataToVerify)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		if((*((uint32_t *)((uint32_t)memAddr) + i)) != (*(dataToVerify + i)))
		{
			return 1;
		}
	}
	
	return 0;
}

/**
	@brief Write Page
	@details Writes a page of data to flash at specified memory address
	@param Memory address to start verify at
	@param Pointer to array of data to write to flash
	@returns success
*/
uint8_t flash_page_write(uint16_t memAddr, uint32_t *dataToWrite)
{
	volatile uint32_t *pagebuf = 0;
	
	// Copy data to write into page buffer at address 0x00
	for (uint8_t i = 0; i < 16; i++)
	{
		*(pagebuf++) = *(dataToWrite++);
	}
	
	//Load address to write
	NVMCTRL->ADDR.reg = memAddr >> 1;
	
	//Write command
	NVMCTRL->CTRLA.reg = (NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP);
	
	//wait for complete
	while(!(NVMCTRL->INTFLAG.bit.READY))
	{
		if(NVMCTRL->INTFLAG.bit.ERROR)
		{
			return 0;
		}
	}
	
	return 1;
}

/**
	@brief Erase Row
	@details Erases an entire row so that it is ready to write
	@param Address of the row to erase
	@returns Success
*/
uint8_t flash_row_erase(uint16_t rowAddr)
{
	//Load address of row to erase
	NVMCTRL->ADDR.reg = rowAddr >> 1;
	
	//Issue erase command
	NVMCTRL->CTRLA.reg = (NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER);
	
	//wait for complete
	while(!(NVMCTRL->INTFLAG.bit.READY))
	{
		if(NVMCTRL->INTFLAG.bit.ERROR)
		{
			return 0;
		}
	}
	
	return 1;
}