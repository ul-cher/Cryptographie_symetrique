#ifndef BREAK_CODE
#define BREAK_CODE

#include "xor.h"

typedef struct {
    unsigned char *key;
    double score_freq;
    int score_dico;
} KeyScore;

typedef struct GenererCle {
    unsigned char** clesValide;
    int* indice;
    int tailleCle;
    unsigned long long nbTotal;
} Generateur;

extern void libererGenerateur(Generateur* genCle);
extern Generateur* break_code_c1(unsigned char* messageCode, int msg_len, int tailleCle);
extern void selectionnerCle(unsigned char* cle, Generateur* genCle);

extern void break_code_c2(KeyScore* tableauCle, unsigned char* message, Generateur* GenererCle, size_t msg_len);
extern void libererTableau(KeyScore* tableauCle, int nbTotal);

extern void break_code_c3(KeyScore* tableauCle, unsigned char* cipher, size_t nbKeys, char* dico_file, int tailleCle, size_t msg_len);
#endif