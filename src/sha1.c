#include "sha1.h"
#include <string.h>

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define F0(b,c,d) ((b & c) | (~b & d))
#define F1(b,c,d) (b ^ c ^ d)
#define F2(b,c,d) ((b & c) | (b & d) | (c & d))
#define F3(b,c,d) (b ^ c ^ d)

static void SHA1_Transform(uint32_t state[5], const uint8_t block[64]) {
    uint32_t w[80];
    uint32_t a, b, c, d, e;

    for (int i = 0; i < 16; i++) {
        w[i]  = (uint32_t)block[i * 4] << 24;
        w[i] |= (uint32_t)block[i * 4 + 1] << 16;
        w[i] |= (uint32_t)block[i * 4 + 2] << 8;
        w[i] |= (uint32_t)block[i * 4 + 3];
    }

    for (int i = 16; i < 80; i++) {
        w[i] = ROTL(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
    }

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    for (int i = 0; i < 80; i++) {
        uint32_t f, k;

        if (i < 20) {
            f = F0(b, c, d);
            k = 0x5A827999;
        } else if (i < 40) {
            f = F1(b, c, d);
            k = 0x6ED9EBA1;
        } else if (i < 60) {
            f = F2(b, c, d);
            k = 0x8F1BBCDC;
        } else {
            f = F3(b, c, d);
            k = 0xCA62C1D6;
        }

        uint32_t temp = ROTL(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = ROTL(b, 30);
        b = a;
        a = temp;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

void SHA1_Init(SHA1_CTX *ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->count[0] = ctx->count[1] = 0;
}

void SHA1_Update(SHA1_CTX *ctx, const uint8_t *data, size_t len) {
    size_t index = (ctx->count[0] >> 3) & 0x3F;

    ctx->count[0] += (uint32_t)(len << 3);
    if (ctx->count[0] < (len << 3))
        ctx->count[1]++;

    ctx->count[1] += (uint32_t)(len >> 29);

    size_t partLen = 64 - index;
    size_t i = 0;

    if (len >= partLen) {
        memcpy(&ctx->buffer[index], data, partLen);
        SHA1_Transform(ctx->state, ctx->buffer);
        for (i = partLen; i + 63 < len; i += 64)
            SHA1_Transform(ctx->state, &data[i]);
        index = 0;
    }

    memcpy(&ctx->buffer[index], &data[i], len - i);
}

void SHA1_Final(uint8_t digest[20], SHA1_CTX *ctx) {
    uint8_t padding[64] = {0x80};
    uint8_t lengthBytes[8];

    for (int i = 0; i < 8; i++)
        lengthBytes[7 - i] = (ctx->count[i >> 2] >> ((i & 3) * 8)) & 0xFF;

    size_t index = (ctx->count[0] >> 3) & 0x3F;
    size_t padLen = (index < 56) ? (56 - index) : (120 - index);

    SHA1_Update(ctx, padding, padLen);
    SHA1_Update(ctx, lengthBytes, 8);

    for (int i = 0; i < 20; i++)
        digest[i] = (ctx->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 0xFF;

    memset(ctx, 0, sizeof(*ctx));
}