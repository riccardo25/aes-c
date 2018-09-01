#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "math_aes.h"

#ifndef SBOX_H
#define SBOX_H

/* TRANSFORMATION*/
uint8_t aff_transf(uint8_t in);
uint8_t inverted_aff_transf(uint8_t in);

/*SBOX DEFINITION*/
uint8_t sbox(uint8_t input, unsigned int enc);

#endif