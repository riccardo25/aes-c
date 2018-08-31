#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "libs/sbox.h"


#ifndef Nb
    #define Nb 4
#endif

#ifndef Nr
    #define Nr 10
#endif

#ifndef Nk
    #define Nk 4
#endif


int Chipher(uint8_t in[4*Nb], uint8_t out[4*Nb], uint32_t w[Nb*(Nr+1)]);
uint32_t generateWord(uint8_t array0, uint8_t array1, uint8_t array2, uint8_t array3 );
int KeyExpantion(uint8_t key[4*Nk], uint32_t w[Nb*(Nr+1)]);
void getBytes(uint32_t input, uint8_t *array );
uint32_t subWord(uint32_t input);
uint32_t rotWord(uint32_t input);
int KeyExpantion(uint8_t key[4*Nk], uint32_t w[Nb*(Nr+1)]);

uint32_t Rcon[] = { 0x01000000,
                    0x02000000,
                    0x04000000,
                    0x08000000,
                    0x10000000,
                    0,20000000,
                    0x40000000,
                    0x80000000,
                    0x1b000000,
                    0x36000000
                    };

int main()
{
    uint8_t prova = 212;
    printf( " %d ", sbox(prova, 0));

    uint8_t in[4 * Nb];
    uint8_t out[4 * Nb];
    uint32_t w[Nb*(Nr+1)];
    uint8_t key[4*Nk] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    KeyExpantion(key, w);


    printf( "\n %08X\n", subWord(0x11001501));
    printf( "\n %08X\n", Rcon[0]);
    //Chipher(in, out, w);



    return 0;
}

int Chipher(uint8_t in[4*Nb], uint8_t out[4*Nb], uint32_t w[Nb*(Nr+1)]){

    uint8_t state[4 * Nb];
    //copy input in state
    for(int i=0; i< 4*Nb; i++){
        *(state + i) = *(in + i); 
        printf(" %d", *(state + i));
    }

    
    

    
    return 0;
}


int KeyExpantion(uint8_t key[4*Nk], uint32_t w[Nb*(Nr+1)]){

    uint32_t temp;
    //fill the first 4, 6 or 8 word with the key
    for (int i = 0; i < Nk; i++)
        w[i] = generateWord(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
    
    //fill the others
    for (int i = Nk; i < Nb*(Nr+1); i++){
        temp = w[i-1];
        if(i % Nk == 0)
            temp = subWord(rotWord(temp)) ^ Rcon[((int)i/Nk)-1];
        else if(Nk > 6 && i % Nk == 4)
            temp = subWord(temp);
        
        w[i] = w[i - Nk] ^ temp;
    }
    return 0;

}

//Takes an array of bytes and generates a word

//word( array[0], array[1], array[2], array[3])
uint32_t generateWord(uint8_t array0, uint8_t array1, uint8_t array2, uint8_t array3 ){
    
    uint32_t uscita = (((uint32_t) array0) << 24) | (((uint32_t) array1) << 16) | (((uint32_t) array2) << 8) | ((uint32_t) array3);
    return uscita;
}

//take an input of 32 bits and separate all bytes
void getBytes(uint32_t input, uint8_t *array ){
    
    array[0] = (uint8_t)((input >> 24) & 0x000000FF);
    array[1] = (uint8_t)((input >> 16) & 0x000000FF);
    array[2] = (uint8_t)((input >> 8) & 0x000000FF);
    array[3] = (uint8_t)(input & 0x000000FF);
}

//takes an 32 bit input and apply SBOX to each byte
uint32_t subWord(uint32_t input)
{
    uint8_t arr[4];
    getBytes(input, arr);
    return generateWord(sbox(arr[0], 1), sbox(arr[1], 1), sbox(arr[2], 1), sbox(arr[3], 1));
}

uint32_t rotWord(uint32_t input)
{
    uint32_t first = (uint8_t)((input >> 24) & 0x000000FF);    
    return (input << 8) | first;
}


