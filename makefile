#Makefile previsto per il progetto AES
#written by Riccardo Fontanini
OBJECTS=

#compiler
CC=gcc

#general flags
CFLAGS= 

#libs

ARGS=fontanini,$(D)

NAMEFILE = main.c
PROGRAM_NAME = aes

LIBS =  
math_aes_lib:
	$(CC) $(CFLAGS) -c libs/math_aes.c -o build/math_aes_lib.o -std=gnu99

sbox_lib:
	$(CC) $(CFLAGS) -c libs/sbox.c -o build/sbox_lib.o -std=gnu99

aes_lib:
	$(CC) $(CFLAGS) -c libs/aes.c -o build/aes_lib.o -std=gnu99

$(PROGRAM_NAME): math_aes_lib sbox_lib aes_lib
	@mkdir -p build
	$(CC) $(CFLAGS) $(NAMEFILE) -o build/$(PROGRAM_NAME) build/math_aes_lib.o build/sbox_lib.o build/aes_lib.o -std=gnu99
	@echo " "
	@echo "Compilazione completata! il file si trova nella cartella build!"
	@echo " "

