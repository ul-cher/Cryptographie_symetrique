#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "break_code.h"
#include "xor.h"

#define ALPHABET_SIZE 26

double freq_th[ALPHABET_SIZE] = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702,
    0.02228, 0.02015, 0.06094, 0.06966, 0.00153,
    0.00772, 0.04025, 0.06749, 0.07507, 0.01929,
    0.05987, 0.00095, 0.06327, 0.09056, 0.02758,
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074,
};

KeyScore initializeKeyScore() {
    KeyScore ks;
    ks.key = NULL;
    ks.score_freq = 0.0;
    ks.score_dico = 0;
    return ks;
}


void libererKeyScore(KeyScore* keyscore) {
    if (keyscore->key != NULL) {
        free(keyscore->key);
    }
    free(keyscore);
}

void libererTableau(KeyScore* tableauCle, int nbTotal) {
    for (int i = 0; i < nbTotal; i++) {
        free(tableauCle[i].key);
    }
    free(tableauCle);
}


void calculerFreq(double* freq, unsigned char* decode) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = 0.0;
    }

    int totalLetters = 0;
    int i = 0;
    while (decode[i] != '\0') {
        if (isalpha(decode[i])) {
            char lettre = tolower(decode[i]);
            freq[lettre - 'a'] += 1.0;
            totalLetters++;
        }
        i++;
    }

    if (totalLetters > 0) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            freq[i] /= totalLetters;
        }
    }
}

double calculerScore(double* freq, double* freq_th) {
    double sommeFreq = 0.0;
    double diff;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        diff = freq_th[i] - freq[i];
        sommeFreq += diff * diff;
    }
    return sommeFreq;
}

void insererTableau(KeyScore scoreCle, KeyScore *tableauCle, int* taille) {
    int pos = 0;
    while (pos <  *taille && tableauCle[pos].score_freq > scoreCle.score_freq) {
        pos++;
    }

    for (int i = *taille; i > pos; i--) {
        tableauCle[i] = tableauCle[i - 1];
    }

    tableauCle[pos] = scoreCle;
    (*taille)++;
}

void break_code_c2(KeyScore* tableauCle, unsigned char* message, Generateur* genCle, size_t msg_len) {
    int taille = 0;
    double* freq = malloc(sizeof(double) * ALPHABET_SIZE);
    if (freq == NULL) {
        perror("Memory allocation error for freq");
        return;
    }

    unsigned char* copieMessage = malloc(sizeof(unsigned char) * (msg_len + 1));
    unsigned char* tempBuffer = malloc(sizeof(unsigned char) * (msg_len + 1));
    if (copieMessage == NULL || tempBuffer == NULL) {
        perror("Memory allocation error for buffers");
        free(freq);
        free(copieMessage);
        free(tempBuffer);
        return;
    }

    // Copy and null-terminate the original message
    memcpy(copieMessage, message, msg_len);
    copieMessage[msg_len] = '\0';

    int lastProgress = -1; // To track the last percentage displayed

    for (int i = 0; i < genCle->nbTotal; i++) {
        KeyScore scoreCle = initializeKeyScore();
        scoreCle.key = malloc(sizeof(unsigned char) * (genCle->tailleCle + 1));
        if (scoreCle.key == NULL) {
            perror("Memory allocation error for scoreCle.key");
            free(freq);
            free(copieMessage);
            free(tempBuffer);
            return;
        }

        // Select a key
        selectionnerCle(scoreCle.key, genCle);

        // XOR cipher on a temporary buffer
        memcpy(tempBuffer, copieMessage, msg_len);
        xor_cipher(tempBuffer, scoreCle.key, msg_len, genCle->tailleCle);
        tempBuffer[msg_len] = '\0';

        // Calculate letter frequencies
        calculerFreq(freq, tempBuffer);

        // Calculate frequency score
        scoreCle.score_freq = calculerScore(freq, freq_th);

        // Insert the score into the sorted key array
        insererTableau(scoreCle, tableauCle, &taille);

        // Display progress every 5%
        int progress = (i + 1) * 100 / genCle->nbTotal;
        if (progress % 5 == 0 && progress != lastProgress) {
            printf("\rProgress: %d%%", progress);
            fflush(stdout);
            lastProgress = progress;
        }
    }

    printf("\n"); // Move to the next line after the loop

    // Free allocated memory
    free(freq);
    free(copieMessage);
    free(tempBuffer);
}
