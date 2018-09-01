/*
    SBOX
    Written by: Riccardo Fontanini
    Start date: 30/8/2018
    Note: SBOX implementation for AES

     R O T A S
     O P E R A
     T E N E T
     A R E P O
     S A T O R

*/

#include "sbox.h"


//if enc is 1 encrypt
uint8_t sbox(uint8_t input, unsigned int enc){
    
    uint8_t a = input;
    if(enc == 0)
        a = inverted_aff_transf(a);
    uint8_t inverted = invert_gf8(a);

    if(enc == 0)
        return inverted;
    return aff_transf(inverted);
    

}



uint8_t aff_transf(uint8_t in) {

    uint8_t uscita[8];

    uint8_t a[8];
    get_bits(in, a);
    
    uint8_t aA = a[0] ^ a[1];
    uint8_t aB = a[2] ^ a[3];
    uint8_t aC = a[4] ^ a[5];
    uint8_t aD = a[6] ^ a[7];
    

    uscita[0] = ~a[0] ^ aC ^ aD;
    uscita[1] = ~a[5] ^ aA ^ aD;
    uscita[2] = a[2] ^ aA ^ aD;
    uscita[3] = a[7] ^ aA ^ aB;
    uscita[4] = a[4] ^ aA ^ aB;
    uscita[5] = ~a[1] ^ aB ^ aC;
    uscita[6] = ~a[6] ^ aB ^ aC;
    uscita[7] = a[3] ^ aC ^ aD;

    return from_bits(uscita);
}


uint8_t inverted_aff_transf(uint8_t in) {

    uint8_t uscita[8];

    uint8_t a[8];
    get_bits(in, a);
    
    uint8_t aA = a[0] ^ a[5];
    uint8_t aB = a[1] ^ a[4];
    uint8_t aC = a[2] ^ a[7];
    uint8_t aD = a[3] ^ a[6];
    

    uscita[0] = ~a[5] ^ aC;
    uscita[1] = a[0] ^ aD;
    uscita[2] = ~a[7] ^ aB;
    uscita[3] = a[2] ^ aA;
    uscita[4] = a[1] ^ aD;
    uscita[5] = a[4] ^ aC;
    uscita[6] = a[3] ^ aA;
    uscita[7] = a[6] ^ aB;

    return from_bits(uscita);
}

