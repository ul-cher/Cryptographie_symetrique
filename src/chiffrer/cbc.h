#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#define TAILLE_BLOC 16

// fonction pour le chiffrement CBC 
void cbc_chiffrement(const unsigned char *message, size_t message_len, const unsigned char *cle, const unsigned char *vi, unsigned char *message_chiffrer);

// fonction pour le déchiffrement CBC
void cbc_dechiffrement(const unsigned char *message_chiffre, size_t message_len, const unsigned char *cle, const unsigned char *vi, unsigned char *message_dechiffre);

// fonction pour le déchiffrement XOR
void xor_chiffrement(const unsigned char *msg, const unsigned char *key, size_t msg_len, size_t key_len, unsigned char *output);