#include "sha.h"

#include <string.h>

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define SHA1_K0 0x5a827999
#define SHA1_K20 0x6ed9eba1
#define SHA1_K40 0x8f1bbcdc
#define SHA1_K60 0xca62c1d6





void hmac_sha1(const char *text, size_t textlen, const char *key, size_t keylen,
    uint8_t digest[SHA1_HASH_BYTES])
{
    sha1_context ctx;
    uint8_t k_pad[SHA1_BLOCK_BYTES];
    uint8_t tk[SHA1_HASH_BYTES];
    unsigned int i;

    if(keylen > SHA1_BLOCK_BYTES) {
        sha1_init(&ctx);
        sha1_update(&ctx, key, keylen);
        sha1_final(&ctx, tk);

        key = tk;
        keylen = SHA1_BLOCK_BYTES;
    }

    memset(k_pad, 0, sizeof(k_pad));
    memcpy(k_pad, key, keylen);
    for(i=0; i < SHA1_BLOCK_BYTES; i++) {
        k_pad[i] ^= 0x36;
    }

    sha1_init(&ctx);
    sha1_update(&ctx, k_pad, SHA1_BLOCK_BYTES);
    sha1_update(&ctx, text, textlen);
    sha1_final(&ctx, digest);

    memset(k_pad, 0, sizeof(k_pad));
    memcpy(k_pad, key, keylen);
    for(i=0; i < SHA1_BLOCK_BYTES; i++) {
        k_pad[i] ^= 0x5c;
    }

    sha1_init(&ctx);
    sha1_update(&ctx, k_pad, SHA1_BLOCK_BYTES);
    sha1_update(&ctx, digest, SHA1_HASH_BYTES);
    sha1_final(&ctx, digest);
}

void sha1_transform(sha1_context *ctx, const uint8_t *data)
{
    uint32_t a, b, c, d, e, i, j, t, m[80];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = ((uint32_t)data[j] << 24) | ((uint32_t)data[j + 1] << 16) | ((uint32_t)data[j + 2] << 8) | ((uint32_t)data[j + 3]);
	for ( ; i < 80; ++i) {
		m[i] = m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16];
		m[i] = ROTLEFT(m[i], 1);
	}

    a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];

    for(i = 0; i < 20; ++i) {
        t = ROTLEFT(a, 5) + ((b & c) ^ (~b & d)) + e + SHA1_K0 + m[i];
        e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
    }    
    for(; i < 40; ++i) {
        t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + SHA1_K20 + m[i];
        e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
    }
    for(; i < 60; ++i) {
        t = ROTLEFT(a, 5) + ((b & c) ^ (b & d) ^ (c & d)) + e + SHA1_K40 + m[i];
        e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
    }
    for(; i < 80; ++i) {
        t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + SHA1_K60 + m[i];
        e = d;
		d = c;
		c = ROTLEFT(b, 30);
		b = a;
		a = t;
    }

    ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;

}

void sha1_init(sha1_context *ctx)
{
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
}

void sha1_update(sha1_context *ctx, const uint8_t *data, size_t len)
{
    uint32_t i;
    for(i = 0; i < len; i++) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if(ctx->datalen == SHA1_BLOCK_BYTES){
            sha1_transform(ctx, ctx->data);
            ctx->bitlen += (SHA1_BLOCK_BYTES*8);
            ctx->datalen = 0;
        }
    }
}

void sha1_final(sha1_context *ctx, uint8_t hash[SHA1_HASH_BYTES])
{
    uint32_t i;
    i = ctx->datalen;

    if(ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while(i < 56)
            ctx->data[i++] = 0x00;
    } else {
        ctx->data[i++] = 0x80;
        while(i < 64)
            ctx->data[i++] = 0x00;
        sha1_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    // Append to the padding the total message's length in bits and transform.
    ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha1_transform(ctx, ctx->data);

    // Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
	}
}
