/*
    Math for aes
    Written by: Riccardo Fontanini
    Start date: 30/8/2018
    Note: AES's math formulas

     R O T A S
     O P E R A
     T E N E T
     A R E P O
     S A T O R

*/


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#ifndef MATH_AES_H
#define MATH_AES_H




/* FUNCTIONS */



/*FOR SBOX*/

//get bits from input to an array of 8 elements
int get_bits(uint8_t input, uint8_t bits[8]) ;

//transform 8 bits to uint8 value
uint8_t from_bits(uint8_t input[8]);

//transformi a GF(8) in GF(4) ah and al 
int map(uint8_t input, uint8_t *ah, uint8_t *al);

uint8_t inverted_map(uint8_t ah, uint8_t al);
uint8_t square_gf4(uint8_t input);
uint8_t mul_gf4(uint8_t in1, uint8_t in2);
uint8_t mule_gf4(uint8_t in);
uint8_t invert_gf4(uint8_t in);
uint8_t invert_gf8(uint8_t in);
uint8_t xtimes(uint8_t in);

/*FOR AES*/

//Takes an array of bytes and generates a word
uint32_t generateWord(uint8_t array0, uint8_t array1, uint8_t array2, uint8_t array3 );
//take an input of 32 bits and separate all bytes
void getBytes(uint32_t input, uint8_t *array );


#endif