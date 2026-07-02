#include "eeprom.h"

#include "sys_io.h"
#include "sys_dbg.h"

uint8_t eeprom_read(uint32_t address, uint8_t* pbuf, uint32_t len) {
	return io_eeprom_read(address, pbuf, len);
}

uint8_t eeprom_write(uint32_t address, uint8_t* pbuf, uint32_t len) {
	return io_eeprom_write(address, pbuf, len);
}

uint8_t eeprom_erase(uint32_t address, uint32_t len) {
	return io_eeprom_erase(address, len);
}


uint8_t DATA_EEPROM_ProgramByte(uint32_t addr, uint8_t data) {
    // TODO: implement actual Flash programming
    return EEPROM_DRIVER_OK;
}

void DATA_EEPROM_Unlock(void) {
        // TODO: add FLASH_Unlock if using STM32 Flash
}
void DATA_EEPROM_Lock(void) {
	// TODO: add FLASH_Lock if using STM32 Flash
}
