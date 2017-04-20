CFLAGS=-g -c
OUTPUT=-o client.exe
CC=/usr/local/bin/gcc.exe

all: client.exe

client.exe: utils.o main.o 
	$(CC) $(OUTPUT) utils.o main.o 

main.o: main.c
	$(CC) $(CFLAGS) main.c

utils.o: utils.c
	$(CC) $(CFLAGS) utils.c