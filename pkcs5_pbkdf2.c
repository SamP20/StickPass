#include "pkcs5_pbkdf2.h"
#include "sha.h"

#include <stdlib.h>
#include <string.h>

#define MINIMUM(a,b) (((a) < (b)) ? (a) : (b))

void zeromem(void *ap, size_t n)
{
    char *p;
    p = ap;
    while(n > 0) {
        *p++ = 0;
        n--;
    }
}

int pkcs5_pbkdf2(const char *pass, size_t passlen, const char *salt,
    size_t saltlen, const char *key, size_t keylen, uint32_t iterations)
{
    uint8_t *asalt, obuf[SHA1_HASH_BYTES];
    uint8_t d1[SHA1_HASH_BYTES], d2[SHA1_HASH_BYTES];
    uint32_t i, j;
    uint32_t iter;
    size_t r;

    if(iterations < 1) return -1;
    if(keylen == 0 || saltlen == 0) return -1;
    if((asalt = malloc(saltlen + 4)) == NULL) return -1;

    memcpy(asalt, salt, saltlen);

    for(iter = 1; keylen > 0; iter++) {
        asalt[saltlen + 0] = (iter >> 24) & 0xff;
        asalt[saltlen + 1] = (iter >> 16) & 0xff;
        asalt[saltlen + 2] = (iter >> 8) & 0xff;
        asalt[saltlen + 3] = iter & 0xff;

        hmac_sha1(asalt, saltlen+4, pass, passlen, d1);
        memcpy(obuf, d1, sizeof(obuf));

        for(i = 1; i<iterations; i++) {
            hmac_sha1(d1, sizeof(d1), pass, passlen, d2);
            memcpy(d1, d2, sizeof(d1));
            for(j = 0; j < sizeof(obuf); j++) {
                obuf[j] ^= d1[j];
            }
        }
        r = MINIMUM(keylen, SHA1_HASH_BYTES);
        memcpy(key, obuf, r);
        key += r;
        keylen -= r;
    }
    //TODO: Check this isn't being optimised out
    zeromem(asalt, saltlen+4);
    free(asalt);
    zeromem(d1, sizeof(d1));
    zeromem(d2, sizeof(d2));
    zeromem(obuf, sizeof(obuf));

    return 0;
}
