#include "math_aes.h"

/* GLOBAL VARIABLES */



/* FUNCTIONS */



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


int get_bits(uint8_t input, uint8_t *bits) {

    for (int i = 0; i<8; i++ ){
        bits[i] = input & 0x01;
        input = input >> 1;
    }
}


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

uint8_t xtimes(uint8_t in){
    uint8_t a[8], uscita[8];
    
    get_bits(in, a);

    // i can also use a shift left and then xor with 00011011;
    uscita[0] = a[7];
    uscita[1] = a[0] ^ a[7];
    uscita[2] = a[1];
    uscita[3] = a[2] ^ a[7];
    uscita[4] = a[3] ^ a[7];
    uscita[5] = a[4];
    uscita[6] = a[5];
    uscita[7] = a[6];

    return from_bits(uscita);

}



//word( array[0], array[1], array[2], array[3])
uint32_t generateWord(uint8_t array0, uint8_t array1, uint8_t array2, uint8_t array3 ){
    
    uint32_t uscita = (((uint32_t) array0) << 24) | (((uint32_t) array1) << 16) | (((uint32_t) array2) << 8) | ((uint32_t) array3);
    return uscita;
}


void getBytes(uint32_t input, uint8_t *array ){
    
    array[0] = (uint8_t)((input >> 24) & 0x000000FF);
    array[1] = (uint8_t)((input >> 16) & 0x000000FF);
    array[2] = (uint8_t)((input >> 8) & 0x000000FF);
    array[3] = (uint8_t)(input & 0x000000FF);
}