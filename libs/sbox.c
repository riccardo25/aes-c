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


//transformi a GF(8) in GF(4) ah and al 
int map(uint8_t input, uint8_t *ah, uint8_t *al){
    uint8_t a[8];
    get_bits(input, a);
    uint8_t aA = a[1] ^ a[7];
    uint8_t aB = a[5] ^ a[7];
    uint8_t aC = a[4] ^ a[6];
    uint8_t a_h[8];
    uint8_t a_l[8];

    //calc al
    a_l[0] = aC ^ a[0] ^ a[5];
    a_l[1] = a[1] ^ a[2];
    a_l[2] = aA;
    a_l[3] = a[2] ^ a[4];

    *al = from_bits(a_l);

    //calc ah
    a_h[0] = aC ^ a[5];
    a_h[1] = aC ^ aA;
    a_h[2] = aB ^ a[2] ^ a[3];
    a_h[3] = aB;

    *ah = from_bits(a_h);
    return 0;
}


uint8_t inverted_map(uint8_t ah, uint8_t al) {
    uint8_t uscita[8];

    uint8_t a_h[8], a_l[8];
    get_bits(ah, a_h);
    get_bits(al, a_l);

    uint8_t aA = a_l[1] ^ a_h[3];
    uint8_t aB = a_h[0] ^ a_h[1];

    uscita[0] = a_l[0] ^ a_h[0];
    uscita[1] = aB ^ a_h[3];
    uscita[2] = aA ^ aB;
    uscita[3] = aB ^ a_l[1] ^ a_h[2];
    uscita[4] = aA ^ aB ^ a_l[3];
    uscita[5] = aB ^ a_l[2];
    uscita[6] = aA ^ a_l[2] ^ a_l[3] ^ a_h[0];
    uscita[7] = aB ^ a_l[2] ^ a_h[3];

    return from_bits(uscita);
}

//get bits from input to an array of 8 elements
int get_bits(uint8_t input, uint8_t *bits) {

    for (int i = 0; i<8; i++ ){
        bits[i] = input & 0x01;
        input = input >> 1;
    }
}

//transform 8 bits to uint8 value
uint8_t from_bits(uint8_t input[8]) {

    uint8_t uscita = 0x00;
    for (int i = 7; i>=0; i-- ){
        uscita = uscita | (input[i] & 0x01);
        uscita = uscita << 1;
    }
}

uint8_t square_gf4(uint8_t input) {
    uint8_t uscita [8];
    uint8_t a[8];

    get_bits(input, a);

    uscita [0] = a[0] ^ a[2];
    uscita [1] = a[2];
    uscita [2] = a[1] ^ a[3];
    uscita [3] = a[3];
    uscita [4] = 0;
    uscita [5] = 0;
    uscita [6] = 0;
    uscita [7] = 0;

    return from_bits(uscita);
    
}

uint8_t mul_gf4(uint8_t in1, uint8_t in2){
    uint8_t uscita[8];

    uint8_t a[8], b[8];
    get_bits(in1, a);
    get_bits(in2, b);

    uint8_t aA = a[0] ^ a[3];
    uint8_t aB = a[2] ^ a[3];

    uscita[0] = (a[0] & b[0]) ^ (a[3] & b[1]) ^ (a[2] & b[2]) ^ (a[1] & b[3]);
    uscita[1] = (a[1] & b[0]) ^ (aA & b[1]) ^ (aB & b[2]) ^ ((a[1] ^ a[2]) & b[3]);
    uscita[2] = (a[2] & b[0]) ^ (a[1] & b[1]) ^ (aA & b[2]) ^ (aB & b[3]);
    uscita[3] = (a[3] & b[0]) ^ (a[2] & b[1]) ^ (a[1] & b[2]) ^ (aA & b[3]);
    uscita[4] = 0;
    uscita[5] = 0;
    uscita[6] = 0;
    uscita[7] = 0;

    return from_bits(uscita);
}

uint8_t mule_gf4(uint8_t in) {
    uint8_t uscita[8];

    uint8_t a[8];
    get_bits(in, a);
    
    uint8_t aA = a[0] ^ a[1];
    uint8_t aB = a[2] ^ a[3];

    uscita[0] = a[1] ^ aB;
    uscita[1] = aA;
    uscita[2] = aA ^ a[2];
    uscita[3] = aA ^ aB;
    uscita[4] = 0;
    uscita[5] = 0;
    uscita[6] = 0;
    uscita[7] = 0;

    return from_bits(uscita);

}

uint8_t invert_gf4(uint8_t in) {

    uint8_t uscita[8];

    uint8_t a[8];
    get_bits(in, a);
    
    uint8_t aA = a[1] ^ a[2] ^ a[3] ^ (a[1] & a[2] & a[3]);
    

    uscita[0] = aA ^ a[0] ^ (a[0] & a[2]) ^ (a[1] & a[2]) ^ (a[0] & a[1] & a[2]);
    uscita[1] = (a[0] & a[1]) ^ (a[0] & a[2]) ^ (a[1] & a[2]) ^ a[3] ^ (a[1] & a[3]) ^ (a[0] & a[1] & a[3]);
    uscita[2] = (a[0] & a[1]) ^ a[2] ^ (a[0] & a[2])  ^ a[3] ^ (a[0] & a[3]) ^ (a[0] & a[2] & a[3]);
    uscita[3] = aA ^ (a[0] & a[3]) ^ (a[1] & a[3]) ^ (a[2] & a[3]);
    uscita[4] = 0;
    uscita[5] = 0;
    uscita[6] = 0;
    uscita[7] = 0;

    return from_bits(uscita);
}


uint8_t invert_gf8(uint8_t in){

    uint8_t ah, al, twoa, twob, three, four, five, six, seven, eight, nine, ten;
    map(in, &ah, &al);
    twoa = square_gf4(ah);
    twob = square_gf4(al);
    three = mul_gf4(ah, al);
    four = ah ^ al;
    five = mule_gf4(twoa);
    six = five ^ twob;
    seven = six ^ three;
    eight = invert_gf4(seven);
    nine = mul_gf4(ah, eight);
    ten = mul_gf4(eight, four);

    return inverted_map(nine, ten);
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