#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "libs/sbox.h"
#include "libs/aes.h"
#include "libs/math_aes.h"

int main()
{
    uint8_t in[4 * Nb] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    
    unsigned int dim_in = 4 * Nb;
    //uint8_t key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
    const unsigned int dim_out = simple_out_dim(dim_in), dim_dec = simple_out_dim(dim_in);
    uint8_t out[dim_out];
    uint8_t dec[dim_dec];
    
    //uint8_t in[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x89};
    //unsigned int dim_in = 4 * Nb+1;
    //char in[] = "ciaomondo";
    //unsigned int dim_in = strlen(in);
    //128
    uint8_t key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    //192
    //uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    aes_encrypt(in, dim_in, out, dim_out, key, 128);

    printf("Encrypted: ");
        for(int i=0; i< dim_out; i++)
            printf(" %02X", *(out + i));
    printf("\n");

    aes_decrypt(out, dim_out, dec, dim_dec, key, 128);

    printf("Decrypted: ");
        for(int i=0; i< dim_dec; i++)
            printf(" %02X", *(dec + i));
    printf("\n");

    //printf("Decrypted: %s\n", dec);

    /*set_key_lenght(192);

    state_transpose(in);
    KeyExpantion(key);
    Chipher(in, out, w);

    state_transpose(out);
    invChipher(out, dec, w);
    state_transpose(dec);*/

    return 0;
}
