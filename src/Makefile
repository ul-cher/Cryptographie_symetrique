CC = gcc
CFLAGS = -Wall -Wextra -I./chiffrer -I./crack
EXECUTABLE = logiciel_final

all: $(EXECUTABLE) chiffrer crack

.PHONY: chiffrer crack clean rebuild

$(EXECUTABLE): logiciel_final.o chiffrer/xor.o chiffrer/cbc.o
	$(CC) $^ -o $(EXECUTABLE)

# Compilation du fichier logiciel_final.c
main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation de xor.c
chiffrer/xor.o: chiffrer/xor.c chiffrer/xor.h
	$(CC) $(CFLAGS) -c $< -o $@

chiffrer/cbc.o: chiffrer/cbc.c chiffrer/cbc.h
	$(CC) $(CFLAGS) -c $< -o $@

# Appel de Makefile dans chiffrer
chiffrer:
	$(MAKE) -C chiffrer

# Appel de Makefile dans crack
crack:
	$(MAKE) -C crack

# Cible pour nettoyer les fichiers objets et l'exécutable
clean:
	$(MAKE) -C chiffrer clean
	$(MAKE) -C crack clean
	rm -f *.o $(EXECUTABLE)

# Cible pour recompiler l'exécutable
rebuild: clean all
