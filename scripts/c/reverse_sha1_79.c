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

output reverse_sha1(int start_round, uint8_t *chunk, uint32_t start_a, uint32_t start_b, uint32_t start_c, uint32_t start_d, uint32_t start_e)
{
    uint32_t a_i_plus_1 = start_a;
    uint32_t b_i_plus_1 = start_b;
    uint32_t c_i_plus_1 = start_c;
    uint32_t d_i_plus_1 = start_d;
    uint32_t e_i_plus_1 = start_e;

    uint32_t a_i;
    uint32_t b_i;
    uint32_t c_i;
    uint32_t d_i;
    uint32_t e_i;

    uint32_t k_i;
    uint32_t f_i_plus_1;

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
        a_i = b_i_plus_1;
        b_i = _right_rotate(c_i_plus_1, 30);
        c_i = d_i_plus_1;
        d_i = e_i_plus_1;

        if (i < 20)
        {
            k_i = 1518500249;
            f_i_plus_1 = d_i ^ (b_i & (c_i ^ d_i));
        }
        else if (i < 40)
        {
            k_i = 1859775393;
            f_i_plus_1 = b_i ^ c_i ^ d_i;
        }
        else if (i < 60)
        {
            k_i = 2400959708;
            f_i_plus_1 = (b_i & c_i) | (b_i & d_i) | (c_i & d_i);
        }
        else
        {
            k_i = 3395469782;
            f_i_plus_1 = b_i ^ c_i ^ d_i;
        }

        e_i = a_i_plus_1 - (_left_rotate(a_i, 5) + f_i_plus_1 + k_i + w[i]);

        printf("i=%d : a = %u, b = %u, c = %u, d = %u, e = %u\n", i, a_i_plus_1, b_i_plus_1, c_i_plus_1, d_i_plus_1, e_i_plus_1);

        a_i_plus_1 = a_i;
        b_i_plus_1 = b_i;
        c_i_plus_1 = c_i;
        d_i_plus_1 = d_i;
        e_i_plus_1 = e_i;        
    }

    output result;

    result.h0 = a_i_plus_1;
    result.h1 = b_i_plus_1;
    result.h2 = c_i_plus_1;
    result.h3 = d_i_plus_1;
    result.h4 = e_i_plus_1;
    result.a = (start_a + a_i);
    result.b = (start_b + b_i);
    result.c = (start_c + c_i);
    result.d = (start_d + d_i);
    result.e = (start_e + e_i);

    return result;
}

int main()
{   
    uint8_t chunk[64] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    uint32_t start_a = 2652194797;
    uint32_t start_b = 1475121602;
    uint32_t start_c = 821726152;
    uint32_t start_d = 3346553428;
    uint32_t start_e = 2817500643;
    int start_round = 79;
    
    output result;
    result = reverse_sha1(start_round, chunk, start_a, start_b, start_c, start_d, start_e);
    
    printf("i=79 : h0 = %u, h1 = %u, h2 = %u, h3 = %u, h4 = %u\n", result.h0, result.h1, result.h2, result.h3, result.h4);
    printf("i=79 (final) : a = %u, b = %u, c = %u, d = %u, e = %u", result.a, result.b, result.c, result.d, result.e);

    return 0;
}