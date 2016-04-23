#ifndef CRYPTO_H
#define CRYPTO_H

#define CRYPTO_TWI_FREQ 100000

#define CRYPTO_KEYSIZE_BITS 128
#define CRYPTO_KEYSIZE_BYTES CRYPTO_KEYSIZE_BITS/2


void Crypto_Init();
void Crypto_Disable();

/** Check if key is unlocked. */
bool Crypto_IsKeyUnlocked(uint8_t id);

/** Attempt to unlock a key. Returns 0 if successful, or
    the number of failed attempts if not. Password ignores upper 8 bits. */
uint8_t Crypto_UnlockKey(uint32_t pass, uint8_t id);

/** loads the active 128 bit key into the key argument, and returns the ID
    of that key */
uint8_t Crypto_GetActiveKey(uint8_t *key);

/** Returns the number of failed password attempts for the specified key id */
uint8_t Crypto_GetPassFailCount(uint8_t id);

// Low level commands for reading/writing secure memory
uint8_t Crypto_WriteCmd(uint8_t cmd, uint8_t addr1, uint8_t addr2, uint8_t* data, uint8_t n);
uint8_t Crypto_WriteSystem(uint8_t addr1, uint8_t addr2, uint8_t* data, uint8_t n);
uint8_t Crypto_WriteUser(uint8_t addr1, uint8_t addr2, uint8_t* data, uint8_t n);
uint8_t Crypto_ReadCmd(uint8_t* data, uint8_t n);

#endif
