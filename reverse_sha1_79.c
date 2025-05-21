#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint32_t _left_rotate(uint32_t n, uint32_t b)
{
    return ((n << b) | (n >> (32 - b)));
}

uint32_t _right_rotate(uint32_t n, uint32_t b)
{
    return ((n >> b) | (n << (32 - b)));
}

typedef struct
{
    uint32_t h0;
    uint32_t h1;
    uint32_t h2;
    uint32_t h3;
    uint32_t h4;
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
} output;

output reverse_sha1(int start_round, uint8_t *chunk, uint32_t a1, uint32_t b1, uint32_t c1, uint32_t d1, uint32_t e1)
{
    uint32_t _a1 = a1;
    uint32_t _b1 = b1;
    uint32_t _c1 = c1;
    uint32_t _d1 = d1;
    uint32_t _e1 = e1;

    uint32_t a0;
    uint32_t b0;
    uint32_t c0;
    uint32_t d0;
    uint32_t e0;

    uint32_t k;
    uint32_t f;

    uint32_t a_equation;

    uint32_t w[80] = {0};

    for (int i = 0; i < 16; i++)
    {
        w[i] = ((uint32_t)chunk[i*4] << 24) | ((uint32_t)chunk[i*4+1] << 16) | ((uint32_t)chunk[i*4+2] << 8) | ((uint32_t)chunk[i*4+3]);
    }

    for (int i = 16; i < 80; i++)
    {
        w[i] = _left_rotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }

    for (int i = start_round; i >= 0; i--)
    {
        a0 = b1;
        b0 = _right_rotate(c1, 30);
        c0 = d1;
        d0 = e1;

        if (i < 20)
        {
            k = 1518500249;
            f = d0 ^ (b0 & (c0 ^ d0));
        }
        else if (i < 40)
        {
            k = 1859775393;
            f = b0 ^ c0 ^ d0;
        }
        else if (i < 60)
        {
            k = 2400959708;
            f = (b0 & c0) | (b0 & d0) | (c0 & d0);
        }
        else
        {
            k = 3395469782;
            f = b0 ^ c0 ^ d0;
        }

        a_equation = _left_rotate(a0, 5) + f + k + w[i];
        e0 = a1 - a_equation;

        printf("i=%d : a = %u, b = %u, c = %u, d = %u, e = %u\n", i, a1, b1, c1, d1, e1);

        a1 = a0;
        b1 = b0;
        c1 = c0;
        d1 = d0;
        e1 = e0;        
    }

    output result;

    result.h0 = a1;
    result.h1 = b1;
    result.h2 = c1;
    result.h3 = d1;
    result.h4 = e1;
    result.a = (_a1 + a0);
    result.b = (_b1 + b0);
    result.c = (_c1 + c0);
    result.d = (_d1 + d0);
    result.e = (_e1 + e0);

    return result;
}

int main()
{   
    uint8_t chunk[64] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    uint32_t a1 = 2652194797;
    uint32_t b1 = 1475121602;
    uint32_t c1 = 821726152;
    uint32_t d1 = 3346553428;
    uint32_t e1 = 2817500643;
    int start_round = 79;
    
    output result;
    result = reverse_sha1(start_round, chunk, a1, b1, c1, d1, e1);
    
    printf("i=79 : h0 = %u, h1 = %u, h2 = %u, h3 = %u, h4 = %u\n", result.h0, result.h1, result.h2, result.h3, result.h4);
    printf("i=79 (final) : a = %u, b = %u, c = %u, d = %u, e = %u", result.a, result.b, result.c, result.d, result.e);

    return 0;
}