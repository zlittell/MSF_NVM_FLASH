#ifndef MSF_NVM_FLASH_H_
#define MSF_NVM_FLASH_H_

void flash_page_read(uint16_t, uint32_t*);
uint8_t flash_page_write_verify(uint16_t, uint32_t*);
uint8_t flash_page_write(uint16_t, uint32_t*);
uint8_t flash_row_erase(uint16_t);

#endif /* MSF_NVM_FLASH_H_ */