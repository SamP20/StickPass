#include "Memory.h"

#include <stdbool.h>
#include <LUFA/Drivers/Peripheral/SPI.h>

void Memory_Init(void) {
    DDRD |= _BV(PD7);
    Memory_Deselect();
    _delay_ms(5);
}

void Memory_PageProgram(uint32_t address, uint8_t* data, uint8_t len) {
    Memory_WriteEnable();
    Memory_Select();
    SPI_SendByte(MEMORY_PP_CMD);
    SPI_SendByte((address>>16) & 0xff);
    SPI_SendByte((address>>8) & 0xff);
    SPI_SendByte(address & 0xff);
    for(;len>0;len--) {
        SPI_SendByte(*(data++));
    }
    Memory_Deselect();
}

void Memory_SectorErase(uint32_t address) {
    Memory_WriteEnable();
    Memory_Select();
    SPI_SendByte(MEMORY_SE_CMD);
    SPI_SendByte((address>>16) & 0xff);
    SPI_SendByte((address>>8) & 0xff);
    SPI_SendByte(address & 0xff);
    Memory_Deselect();
}

void Memory_BlockErase(uint32_t address) {
    Memory_WriteEnable();
    Memory_Select();
    SPI_SendByte(MEMORY_BE_CMD);
    SPI_SendByte((address>>16) & 0xff);
    SPI_SendByte((address>>8) & 0xff);
    SPI_SendByte(address & 0xff);
    Memory_Deselect();
}

void Memory_ChipErase(void) {
    Memory_WriteEnable();
    Memory_Select();
    SPI_SendByte(MEMORY_CE_CMD);
    Memory_Deselect();
}

void Memory_Read(uint32_t address, uint8_t* data, uint32_t len) {
    Memory_WaitForWrite();
    Memory_Select();
    SPI_SendByte(MEMORY_READ_CMD);
    SPI_SendByte((address>>16) & 0xff);
    SPI_SendByte((address>>8) & 0xff);
    SPI_SendByte(address & 0xff);
    for(;len>0;len--) {
        *(data++) = SPI_ReceiveByte();
    }
    Memory_Deselect();
}

void Memory_WaitForWrite(void) {
    uint8_t status;
    while(true) {
        Memory_Select();
        SPI_SendByte(MEMORY_RDSR1_CMD);
        status = SPI_ReceiveByte();
        Memory_Deselect();
        if(status & MEMORY_WIP_STATUS1) {
            _delay_ms(5);
        } else {
            break;
        }
    }
}

void Memory_WriteEnable(void) {
    Memory_WaitForWrite();
    Memory_Select();
    SPI_SendByte(MEMORY_WREN_CMD);
    Memory_Deselect();
}
