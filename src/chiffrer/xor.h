#ifndef XOR_H
#define XOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


// Fonction pour générer une clé alphanumérique aléatoire
void gen_key(unsigned char *key, size_t length);


// Fonction pour chiffrer et déchiffrer avec XOR
void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len, size_t key_len);

// Fonction pour suvegarder le clé
void save_key_to_file(const char *filename, unsigned char *key);
#endif