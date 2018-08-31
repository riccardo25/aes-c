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

sbox:
	$(CC) $(CFLAGS) -c libs/sbox.c -o build/sbox.o -std=gnu99

$(PROGRAM_NAME): sbox
	@mkdir -p build
	$(CC) $(CFLAGS) $(NAMEFILE) -o build/$(PROGRAM_NAME) build/sbox.o -std=gnu99
	@echo " "
	@echo "Compilazione completata! il file si trova nella cartella build!"
	@echo " "

