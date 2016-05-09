#ifndef MEMORY_H
#define MEMORY_H

#include <avr/io.h>
#include <stdint.h>

#define MEMORY_WREN_CMD 0x06
#define MEMORY_WRDI_CMD 0x04
#define MEMORY_RDSR1_CMD 0x05
#define MEMORY_RDSR2_CMD 0x35
#define MEMORY_WRSR_CMD 0x01
#define MEMORY_READ_CMD 0x03
#define MEMORY_FAST_READ_CMD 0x0B
#define MEMORY_ROTP_CMD 0x4B
#define MEMORY_POTP_CMD 0x42
#define MEMORY_PP_CMD 0x02
#define MEMORY_SE_CMD 0x20
#define MEMORY_BE_CMD 0xD8
#define MEMORY_CE_CMD 0xC7
#define MEMORY_DP_CMD 0xB9
#define MEMORY_RDID_CMD 0x9F
#define MEMORY_REMS_CMD 0x90
#define MEMORY_RES_CMD 0xAB
#define MEMORY_HPM_CMD 0xA3

#define MEMORY_WIP_STATUS1 0x01
#define MEMORY_WEL_STATUS1 0x02
#define MEMORY_BP0_STATUS1 0x04
#define MEMORY_BP1_STATUS1 0x08
#define MEMORY_BP2_STATUS1 0x10
#define MEMORY_TB_STATUS1 0x20
#define MEMORY_SEC_STATUS1 0x40
#define MEMORY_SRP0_STATUS1 0x80

#define MEMORY_SRP1_STATUS2 0x01
#define MEMORY_APT_STATUS2 0x04
#define MEMORY_CMP_STATUS2 0x40


void Memory_Init(void);

static inline void Memory_Select(void) {
    PORTD &= ~_BV(PD7);
}

static inline void Memory_Deselect(void) {
    PORTD |= _BV(PD7);
}

void Memory_PageProgram(uint32_t address, uint8_t* data, uint8_t len);
void Memory_SectorErase(uint32_t address);
void Memory_BlockErase(uint32_t address);
void Memory_ChipErase(void);

void Memory_Read(uint32_t address, uint8_t* data, uint32_t len);

// Do not call these commands directly. They are called by the program/erase functions
void Memory_WaitForWrite(void);
void Memory_WriteEnable(void);

#endif
