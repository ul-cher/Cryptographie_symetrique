#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#define TAILLE_BLOC 16

// fonction XOR upgrated (parametre output ajouté)
void xor_chiffrement(const unsigned char *msg, const unsigned char *key, size_t msg_len, size_t key_len, unsigned char *output) {
    for (size_t i = 0; i < msg_len; i++) {
        output[i] = msg[i] ^ key[i % key_len];
    }
}

// Fonction pour le chiffrement CBC 
void cbc_chiffrement(unsigned char *message, size_t message_len, unsigned char *cle, const unsigned char *iv, unsigned char *message_chiffrer) {
   // la longueur ajustée (pour que le message soit un multiple de TAILLE_BLOC)
    size_t len_aligned = (message_len % TAILLE_BLOC == 0) ? message_len : message_len + (TAILLE_BLOC - message_len % TAILLE_BLOC);
    unsigned char bloc_precedent[TAILLE_BLOC];

    // initialisation avec le vecteur IV
    memcpy(bloc_precedent, iv, TAILLE_BLOC);

    for (size_t offset = 0; offset < len_aligned; offset += TAILLE_BLOC) {
        unsigned char bloc_courant[TAILLE_BLOC] = {0}; 

        // remplissage du bloc avec les données, si nécessaire
        size_t copy_len = (offset + TAILLE_BLOC <= message_len) ? TAILLE_BLOC : message_len - offset;
        memcpy(bloc_courant, message + offset, copy_len);

        // XOR avec le bloc précédent (ou IV pour le premier bloc)
        for (size_t j = 0; j < TAILLE_BLOC; j++) {
            bloc_courant[j] ^= bloc_precedent[j];
        }

        // chiffrement du bloc courant
        size_t key_len = strlen((const char *)cle);
        xor_chiffrement(bloc_courant, cle, TAILLE_BLOC, key_len, message_chiffrer + offset);

        // mise à jour du bloc précédent
        memcpy(bloc_precedent, message_chiffrer + offset, TAILLE_BLOC);
    }
}

// Fonction de déchiffrement CBC
void cbc_dechiffrement(unsigned char *message_chiffre, size_t message_len, unsigned char *cle, const unsigned char *iv, unsigned char *message_dechiffre) {
    unsigned char bloc_precedent[TAILLE_BLOC];

    // initialisation avec le vecteur IV
    memcpy(bloc_precedent, iv, TAILLE_BLOC);

    for (size_t offset = 0; offset < message_len; offset += TAILLE_BLOC) {
        unsigned char bloc_dechiffre[TAILLE_BLOC];
        unsigned char bloc_intermediaire[TAILLE_BLOC];

        // Sauvegarde du bloc chiffré courant pour mise à jour du bloc précédent
        unsigned char bloc_chiffre_courant[TAILLE_BLOC];
        memcpy(bloc_chiffre_courant, message_chiffre + offset, TAILLE_BLOC);

        // Déchiffrement du bloc courant
        size_t key_len = strlen((const char *)cle);
        xor_chiffrement(message_chiffre + offset, cle, TAILLE_BLOC, key_len, bloc_intermediaire);

        // XOR avec le bloc précédent (ou IV pour le premier bloc)
        for (size_t i = 0; i < TAILLE_BLOC; i++) {
            bloc_dechiffre[i] = bloc_intermediaire[i] ^ bloc_precedent[i];
        }

        // Copie du bloc déchiffré dans le tampon de sortie
        memcpy(message_dechiffre + offset, bloc_dechiffre, TAILLE_BLOC);

        // Mise à jour du bloc précédent avec le bloc chiffré courant
        memcpy(bloc_precedent, bloc_chiffre_courant, TAILLE_BLOC);
    }
}