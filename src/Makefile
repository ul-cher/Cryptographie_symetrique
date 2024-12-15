CC = gcc
CFLAGS = -I./chiffrer -Wall -Wextra

all: main

main: main.c chiffrer/cbc.c chiffrer/xor.c
	$(CC) $(CFLAGS) main.c chiffrer/cbc.c chiffrer/xor.c -o main

clean:
	rm -f main
