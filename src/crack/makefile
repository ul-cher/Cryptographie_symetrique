# Nom de l'exécutable final
TARGET = break_code

# Fichiers source
SRCS = break_code_c1.c break_code_c2.c break_code_c3.c break_code_main.c xor.c

# Fichiers d'en-tête
HEADERS = break_code_c1.h break_code_c2.h break_code_c3.h xor.h

# Fichiers objets (générés à partir des fichiers source)
OBJS = $(SRCS:.c=.o)

# Compilateur et options
CC = gcc
CFLAGS = -Wall -g

# Règle par défaut pour compiler l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Règle pour compiler les fichiers .c en fichiers .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets pour éviter des conflits avec des fichiers du même nom
.PHONY: clean
