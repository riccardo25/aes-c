#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifndef SBOX_H
#define SBOX_H

/*MATHS FOR AES*/
int get_bits(uint8_t input, uint8_t bits[8]) ;
uint8_t from_bits(uint8_t input[8]);
int map(uint8_t input, uint8_t *ah, uint8_t *al);
uint8_t inverted_map(uint8_t ah, uint8_t al);
uint8_t square_gf4(uint8_t input);
uint8_t mul_gf4(uint8_t in1, uint8_t in2);
uint8_t mule_gf4(uint8_t in);
uint8_t invert_gf4(uint8_t in);
uint8_t invert_gf8(uint8_t in);

/* TRANSFORMATION*/
uint8_t aff_transf(uint8_t in);
uint8_t inverted_aff_transf(uint8_t in);

/*SBOX DEFINITION*/
uint8_t sbox(uint8_t input, unsigned int enc);

#endif