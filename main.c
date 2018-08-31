#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "libs/sbox.h"


int main()
{
    uint8_t prova = 212;
    printf( " %d ", sbox(prova, 0));
    return 0;
}

