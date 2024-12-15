EXEC = dh_gen_group

SRC = dh_gen_group.c dh_prime.c

CFLAGS = -Wall

CC = gcc

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

clean:
	@rm -f $(EXEC)

distclean: clean
	@rm -f *.o
