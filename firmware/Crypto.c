#include "Crypto.h"

#include <LUFA/Drivers/Peripheral/TWI.h>
#include <string.h>

static uint8_t activeKey[CRYPTO_KEYSIZE_BYTES] = {0x00};
static uint8_t passID = 0xff; //0xff indicates no valid key

void Crypto_Init(void) {
    TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, CRYPTO_TWI_FREQ));
}

void Crypto_Disable(void) {
    TWI_Disable();
}

bool Crypto_IsKeyUnlocked(uint8_t id) {
    return passID == id;
}

uint8_t Crypto_UnlockKey(uint32_t pass, uint8_t id, bool write) {
    uint8_t data[3];
    data[0] = (pass & 0xff);
    data[1] = ((pass>>8) & 0xff);
    data[2] = ((pass>>16) & 0xff);
    if(!write) id += 0x10;
    // Verify password command
    Crypto_WriteCmd(0xBA, id, 0x00, data, 3);
    // Dummy poll command
    Crypto_WriteCmd(0xB6, 0x00, 0x00, NULL, 0);

    return Crypto_GetPassFailCount(id, write);
}

uint8_t Crypto_GetActiveKey(uint8_t *key) {
    memcpy(key, activeKey, CRYPTO_KEYSIZE_BYTES);
    return passID;
}

uint8_t Crypto_GetPassFailCount(uint8_t id, bool write) {
    uint8_t passAddr = 0xB0;
    passAddr += id*0x08;
    if(write) passAddr += 0x04;
    if(Crypto_WriteSystem(0x00, passAddr, NULL, 0) == TWI_ERROR_NoError) {
        uint8_t result;
        if(Crypto_ReadCmd(&result, 1) == TWI_ERROR_NoError) {
            return ((result)&1) + ((result>>1)&1) +
                   ((result>>2)&1) + ((result>>3)&1);
        }
    }

    return 0xff;
}

uint8_t Crypto_WriteCmd(uint8_t cmd, uint8_t addr1, uint8_t addr2, uint8_t* data, uint8_t n) {
    uint8_t status = TWI_StartTransmission(cmd, 10);
    if(status == TWI_ERROR_NoError) {
        // Dummy write
        TWI_SendByte(addr1);
        TWI_SendByte(addr2);
        TWI_SendByte(n);
        for(;n>0;n--) {
            TWI_SendByte(*(data++));
        }
        TWI_StopTransmission();
    }
    _delay_ms(5);
    return status;
}

uint8_t Crypto_WriteSystem(uint8_t addr1, uint8_t addr2, uint8_t* data, uint8_t n) {
    return Crypto_WriteCmd(0xB4, addr1, addr2, data, n);
}

uint8_t Crypto_WriteUser(uint8_t addr1, uint8_t addr2, uint8_t* data, uint8_t n) {
    return Crypto_WriteCmd(0xB0, addr1, addr2, data, n);
}

uint8_t Crypto_ReadCmd(uint8_t* data, uint8_t n) {
    uint8_t status = TWI_StartTransmission(0xB1 | TWI_ADDRESS_READ, 10);
    if (status == TWI_ERROR_NoError) {
        for(;n>0;n--) {
            TWI_ReceiveByte(data++, n==1?true:false);
        }
        TWI_StopTransmission();
    }
    return status;
}
