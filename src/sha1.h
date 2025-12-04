#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t state[5];   // A, B, C, D, E  (SHA-1 internal state)
    uint32_t count[2];   // Number of processed bits
    uint8_t buffer[64];  // 512-bit block buffer
} SHA1_CTX;

void SHA1_Init(SHA1_CTX *ctx);
void SHA1_Update(SHA1_CTX *ctx, const uint8_t *data, size_t len);
void SHA1_Final(uint8_t digest[20], SHA1_CTX *ctx);

#endif