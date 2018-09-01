/*
    AES
    Written by: Riccardo Fontanini
    Start date: 30/8/2018
    Note: Implementation for AES

     R O T A S
     O P E R A
     T E N E T
     A R E P O
     S A T O R

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "math_aes.h"
#include "sbox.h"

#ifndef EAS_H
#define EAS_H

/* GLOBAL VARIABLES */

#ifndef Nb
    #define Nb 4
#endif

/* GLOBAL VARIABLES */
extern unsigned int Nk;

extern unsigned int Nr;

extern uint32_t *w;

extern uint32_t Rcon[];

/*FUNCTIONS*/

//crypt function interface
int aes_encrypt(uint8_t *in, unsigned int len_in, uint8_t *out, unsigned int len_out, uint8_t *key, unsigned int key_bits);

//cipher of aes
int Chipher(uint8_t in[4*Nb], uint8_t out[4*Nb], uint32_t *w);
//creates keys for aes
int KeyExpantion(uint8_t *key);

//set the lenght of the key
void set_key_lenght(unsigned int dim);

//get the key lenght -1 for error
int get_key_lenght();

//takes an 32 bit input and apply SBOX to each byte
uint32_t subWord(uint32_t input);
uint32_t rotWord(uint32_t input);

void AddRoundKey(uint8_t *state, uint32_t *w, unsigned int first);
void subBytes(uint8_t *state);
void shiftRows(uint8_t *state);
void swaprow(uint8_t *state, unsigned int row);
void mixColumns(uint8_t *state);




#endif