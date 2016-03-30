#ifndef PKCS5_PBKDF2
#define PKCS5_PBKDF2

#include <stdint.h>
#include <stddef.h>

int pkcs5_pbkdf2(const char *pass, size_t passlen,
                 const char *salt, size_t saltlen,
                 const char *key, size_t keylen,
                 uint32_t iterations);

#endif
