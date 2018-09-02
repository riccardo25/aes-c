#include "aes.h"

/* GLOBAL VARIABLES */

uint32_t *w = NULL;

unsigned int Nk = 4;

unsigned int Nr = 10;

uint32_t Rcon[] = { 0x01000000,
                    0x02000000,
                    0x04000000,
                    0x08000000,
                    0x10000000,
                    0x20000000,
                    0x40000000,
                    0x80000000,
                    0x1b000000,
                    0x36000000
                    };

/* FUNCTIONS*/

//transpose state
void state_transpose(uint8_t *state) {
    
    uint8_t newstate[Nb*4];

    for (int i = 0; i < Nb; i++)
        for(int j = 0; j < Nb ; j++)
            *(newstate + Nb*i + j) = *(state + Nb*j + i);
        

    for (int i = 0; i < Nb*4; i++)
        *(state + i) = *(newstate + i);
}

/*ENCRYPT*/
//return -1 bed params
//if len not multiple of 4*Nb, zeros will be added at the end
//len_in coud what you want, but len_outmust be multiple of Nb*4 and >= len_in
int aes_encrypt(uint8_t *in, unsigned int len_in, uint8_t *out, unsigned int len_out, uint8_t *key, unsigned int key_bits){

    //control parameters
    if(len_in == 0 || (key_bits != 128 && key_bits != 192 && key_bits != 256 ) || len_out < len_in || len_out % 4*Nb != 0)
        return -1;

    //get the number of elements in the tail of input buffer (add 0)
    unsigned int tail = len_in % (4*Nb);
    unsigned int N = len_in / (4*Nb);//number of iterations
    if(tail != 0)//one more iteration
        N++;
    //key settings
    set_key_lenght(key_bits);
    KeyExpantion(key);

    uint8_t *output;
    uint8_t newin[ 4 * Nb];

    for (int i = 0; i < N; i++){
        //transpose and full tail with 0
        for(int c = 0; c < Nb*4 ; c++){
            if( i != N-1 || tail == 0 || c < tail)
                *(newin + Nb * (c % 4) + (int) (c / 4)) = *(in + i * 4 * Nb + c);
            else 
                *(newin + Nb * (c % 4) + (int) (c / 4)) = 0;
        }

        output = out + i * 4 * Nb;

        //encrypt data
        Chipher(newin, output, w);
    }

    return 0;
}


int Chipher(uint8_t in[4*Nb], uint8_t out[4*Nb], uint32_t *w){

    uint8_t state[4 * Nb];
    //copy input in state
    for(int i=0; i< 4*Nb; i++)
        *(state + i) = *(in + i); 

    AddRoundKey(state, w, 0);

    /*printf("After addroundkey");
    for(int i=0; i< 4*Nb; i++)
        printf(" %02X", *(state + i));
    
    printf(" \n");*/

    for(int i = 1; i< Nr; i++)
    {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        AddRoundKey(state, w, i*Nb);
        
    
        /*printf("After round %d:", i);
        for(int c=0; c< 4*Nb; c++)
            printf(" %02X", *(state + c));
        printf(" \n");*/
    }

    subBytes(state);
    shiftRows(state);
    AddRoundKey(state, w, Nr*Nb);
    
    for(int i=0; i < Nb; i++)
        for (int j = 0; j < Nb; j++)
            *(out + Nb*i + j) = *(state + Nb*j + i);

    return 0;
}


int KeyExpantion(uint8_t *key){

    if(w == NULL){
        //uint32_t w[Nb*(Nr+1)]
        w = (uint32_t*) malloc(Nb*(Nr+1) * sizeof(uint32_t));

    }

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


void set_key_lenght(unsigned int dim) {
    
    if (dim == 192) {
        Nk = 6;
        Nr = 12;
    }
    else if(dim == 256) {
        Nk = 8;
        Nr = 14;
    }
    else {
        Nk = 4;
        Nr = 10;
    }
}

int get_key_lenght(){
    if (Nk == 6) 
        return 192;
    else if(Nk == 8) 
        return 256;
    else if(Nk == 4)
        return 128;
    else 
        return -1;
    
}

uint32_t subWord(uint32_t input){
    uint8_t arr[4];
    getBytes(input, arr);
    return generateWord(sbox(arr[0], 1), sbox(arr[1], 1), sbox(arr[2], 1), sbox(arr[3], 1));
}

uint32_t rotWord(uint32_t input){
    uint32_t first = (uint8_t)((input >> 24) & 0x000000FF);    
    return (input << 8) | first;
}


void AddRoundKey(uint8_t *state, uint32_t *w, unsigned int first){
    
    uint8_t temp[4];
    for(int i = 0; i< Nb; i++){
        getBytes(*(w + first + i), temp);

        // XOR for column
        *(state + i + 0) = *(state + i + 0) ^ temp[0];
        *(state + i + 1*Nb) = *(state + i + 1*Nb) ^ temp[1];
        *(state + i + 2*Nb) = *(state + i + 2*Nb) ^ temp[2];
        *(state + i + 3*Nb) = *(state + i + 3*Nb) ^ temp[3];
    }
}


void subBytes(uint8_t *state){
    for (int i=0; i<Nb*4; i++)
        *(state + i) = sbox(*(state + i), 1);
}

void shiftRows(uint8_t *state){
    //shift row 1, 1times
    swaprow(state, 1);
    //shift row 2 2times
    swaprow(state, 2);
    swaprow(state, 2);
    //shift row 3 3times
    swaprow(state, 3);
    swaprow(state, 3);
    swaprow(state, 3);
}

void swaprow(uint8_t *state, unsigned int row){
    uint8_t swap = *(state + Nb * row);

    *(state + Nb * row) = *(state + Nb * row + 1);
    *(state + Nb * row + 1) = *(state + Nb * row + 2);
    *(state + Nb * row + 2) = *(state + Nb * row +3);
    *(state + Nb * row + 3) = swap;
}

void mixColumns(uint8_t *state){

    uint8_t newstate[4 * Nb];
    for(int i=0; i<4; i++){
        //S0i                               02 * S0i                                        03 *S1i                                 S2i                         S3i
        *(newstate + Nb * 0 + i) = xtimes(*(state + Nb * 0 + i)) ^ mul03(*(state + Nb * 1 + i))  ^ *(state + Nb * 2 + i) ^ *(state + Nb * 3 + i);
        //S1i                               S0i                    02 * S1i                                             03 *S2i                               S3i
        *(newstate + Nb * 1 + i) = *(state + Nb * 0 + i) ^ xtimes(*(state + Nb * 1 + i)) ^ mul03(*(state + Nb * 2 + i))  ^ *(state + Nb * 3 + i);

        *(newstate + Nb * 2 + i) = *(state + Nb * 0 + i) ^ *(state + Nb * 1 + i) ^ xtimes(*(state + Nb * 2 + i)) ^ mul03(*(state + Nb * 3 + i)) ;

        *(newstate + Nb * 3 + i) = mul03(*(state + Nb * 0 + i))  ^ *(state + Nb * 1 + i) ^ *(state + Nb * 2 + i) ^ xtimes(*(state + Nb * 3 + i));
    }

    for( int i = 0; i< Nb*4; i++)
        *(state + i) = *(newstate + i);
}



/*DECRYPT*/


//return -1 bed params
//if len not multiple of 4*Nb, zeros will be added at the end
//len_in coud what you want, but len_outmust be multiple of Nb*4 and >= len_in
int aes_decrypt(uint8_t *in, unsigned int len_in, uint8_t *out, unsigned int len_out, uint8_t *key, unsigned int key_bits){

    //control parameters
    if(len_in == 0 || (key_bits != 128 && key_bits != 192 && key_bits != 256 ) || len_out < len_in || len_out % 4*Nb != 0)
        return -1;

    //get the number of elements in the tail of input buffer (add 0)
    unsigned int tail = len_in % (4*Nb);
    unsigned int N = len_in / (4*Nb);//number of iterations
    if(tail != 0)//one more iteration
        N++;
    //key settings
    set_key_lenght(key_bits);
    KeyExpantion(key);

    uint8_t *output;
    uint8_t newin[ 4 * Nb];

    for (int i = 0; i < N; i++){
        //transpose and full tail with 0
        for(int c = 0; c < Nb*4 ; c++){
            if( i != N-1 || tail == 0 || c < tail)
                *(newin + Nb * (c % 4) + (int) (c / 4)) = *(in + i * 4 * Nb + c);
            else 
                *(newin + Nb * (c % 4) + (int) (c / 4)) = 0;
        }

        output = out + i * 4 * Nb;

        //encrypt data
        invChipher(newin, output, w);
    }

    return 0;
}


int invChipher(uint8_t in[4*Nb], uint8_t out[4*Nb], uint32_t *w){

    uint8_t state[4 * Nb];
    //copy input in state
    for(int i=0; i< 4*Nb; i++)
        *(state + i) = *(in + i); 
        
    AddRoundKey(state, w, Nr*Nb);

    /*printf("After addroundkey");
    for(int i=0; i< 4*Nb; i++)
        printf(" %02X", *(state + i));
    
    printf(" \n");*/

    for(int i = Nr-1; i >= 1; i--)
    {
        invShiftRows(state);
        invSubBytes(state);
        AddRoundKey(state, w, i*Nb);
        invMixColumns(state);
        
        /*printf("After round %d:", i);
        for(int i=0; i< 4*Nb; i++)
            printf(" %02X", *(state + i));
    
        printf(" \n");*/
    }

    invShiftRows(state);
    invSubBytes(state);
    AddRoundKey(state, w, 0);
    
    for(int i=0; i < Nb; i++)
        for (int j = 0; j < Nb; j++)
            *(out + Nb*i + j) = *(state + Nb*j + i);

    return 0;
}



void invShiftRows(uint8_t *state){
    //shift row 1, 3times
    swaprow(state, 1);
    swaprow(state, 1);
    swaprow(state, 1);
    //shift row 2 2times
    swaprow(state, 2);
    swaprow(state, 2);
    //shift row 3 1times
    swaprow(state, 3);
}

void invMixColumns(uint8_t *state){

    uint8_t newstate[4 * Nb];
    for(int i=0; i<4; i++){
    
        *(newstate + Nb * 0 + i) = mul0E(*(state + Nb * 0 + i)) ^ mul0B(*(state + Nb * 1 + i)) ^ mul0D( *(state + Nb * 2 + i)) ^ mul09(*(state + Nb * 3 + i));

        *(newstate + Nb * 1 + i) = mul09(*(state + Nb * 0 + i)) ^ mul0E(*(state + Nb * 1 + i)) ^ mul0B( *(state + Nb * 2 + i)) ^ mul0D(*(state + Nb * 3 + i));

        *(newstate + Nb * 2 + i) = mul0D(*(state + Nb * 0 + i)) ^ mul09(*(state + Nb * 1 + i)) ^ mul0E( *(state + Nb * 2 + i)) ^ mul0B(*(state + Nb * 3 + i));

        *(newstate + Nb * 3 + i) = mul0B(*(state + Nb * 0 + i)) ^ mul0D(*(state + Nb * 1 + i)) ^ mul09( *(state + Nb * 2 + i)) ^ mul0E(*(state + Nb * 3 + i));
    }

    for( int i = 0; i< Nb*4; i++)
        *(state + i) = *(newstate + i);
}

void invSubBytes(uint8_t *state){
    for (int i=0; i<Nb*4; i++)
        *(state + i) = sbox(*(state + i), 0);
}