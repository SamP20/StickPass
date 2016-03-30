#ifndef SHA_H_INCLUDED
#define SHA_H_INCLUDED

#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stddef.h>

#define SHA1_HASH_BYTES 20
#define SHA1_BLOCK_BYTES 64

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[5];
} sha1_context;

void hmac_sha1(const char *text, size_t textlen, const char *key, size_t keylen,
    uint8_t digest[SHA1_HASH_BYTES]);

void sha1_init(sha1_context *ctx);
void sha1_update(sha1_context *ctx, const uint8_t *data, size_t len);
void sha1_final(sha1_context *ctx, uint8_t hash[SHA1_HASH_BYTES]);

#endif


#endif // SHA_H_INCLUDED
