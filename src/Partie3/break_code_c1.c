#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "break_code.h"

#define MAX_CHARACTERS 62
#define CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

void libererCles(int tailleCle, unsigned char** cles) {
    if (cles) {
        for (int i = 0; i < tailleCle; i++) {
            free(cles[i]);
        }
        free(cles);
    }
}

void libererGenerateur(Generateur* genCle) {
    if (genCle) {
        libererCles(genCle->tailleCle, genCle->clesValide);
        free(genCle->indice);
        free(genCle);
        genCle = NULL;
    }
}

void supprimer_caractere(unsigned char *chaine, unsigned char caractere) {
    int j = 0;
    size_t len = strlen((const char*)chaine);
    unsigned char temp[len + 1];
    for (size_t i = 0; i < len; i++) {
        if (chaine[i] != caractere) {
            temp[j++] = chaine[i];
        }
    }
    temp[j] = '\0';
    strcpy((char*)chaine, (const char*)temp);
}

int iterateurNext(Generateur* genCle) {
    int fin = 1;
    for (int i = genCle->tailleCle-1; i >= 0; i--) {
        if (genCle->indice[i] + 1 < (int)strlen((const char*)genCle->clesValide[i])) {
            genCle->indice[i]++;
            fin = 0;
            break;
        } else {
            genCle->indice[i] = 0;
        }
    }
    return fin;
}

void selectionnerCle(unsigned char* cle, Generateur* genCle) {
    for (int i = 0; i < genCle->tailleCle; i++) {
        cle[i] = genCle->clesValide[i][genCle->indice[i]];
    }
    cle[genCle->tailleCle] = '\0';
    iterateurNext(genCle);
}

int verifierCharactere(unsigned char carCrypte, unsigned char carCle) {
    unsigned char carDecode = carCrypte ^ carCle;
    return isalnum(carDecode) || isspace(carDecode) || ispunct(carDecode);
}


Generateur* break_code_c1(unsigned char* messageCode, int msg_len, int tailleCle) {
    unsigned char** cles = malloc(sizeof(unsigned char*) * tailleCle);
    if (cles == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }

    for (int i = 0; i < tailleCle; i++) {
        cles[i] = malloc((MAX_CHARACTERS + 1) * sizeof(unsigned char));
        if (cles[i] == NULL) {
            perror("Erreur d'allocation mémoire");
            libererCles(i, cles);
            return NULL;
        }
        strcpy((char*)cles[i], CHARSET);
    }

    for (size_t i = 0; i < msg_len; i++) {
        int j = 0;
        while (cles[i % tailleCle][j] != '\0') {
            if (!verifierCharactere(messageCode[i], cles[i % tailleCle][j])) {
                supprimer_caractere(cles[i % tailleCle], cles[i % tailleCle][j]);
            } else {
                j++;
            }
        }
    }

    Generateur* generateurCle = malloc(sizeof(Generateur));
    if (generateurCle == NULL) {
        perror("Erreur d'allocation mémoire");
        libererCles(tailleCle, cles);
        return NULL;
    }

    generateurCle->clesValide = cles;
    generateurCle->tailleCle = tailleCle;

    generateurCle->indice = malloc(sizeof(int) * tailleCle);
    if (generateurCle->indice == NULL) {
        perror("Erreur d'allocation mémoire");
        libererCles(tailleCle, cles);
        free(generateurCle);
        return NULL;
    }
    memset(generateurCle->indice, 0, sizeof(int) * tailleCle);

    generateurCle->nbTotal = 1;
    for (int i = 0; i < tailleCle; i++) {
        generateurCle->nbTotal *= strlen((const char*)cles[i]);
    }

    return generateurCle;
}
