#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "break_code.h"
#include "xor.h"

// Fonction de comparaison pour trier les KeyScore par score_dico
int comparer_score_dico(const void* a, const void* b) {
    KeyScore* keyscore_a = (KeyScore*)a;
    KeyScore* keyscore_b = (KeyScore*)b;

    if (keyscore_a->score_dico < keyscore_b->score_dico) {
        return -1;
    } else if (keyscore_a->score_dico > keyscore_b->score_dico) {
        return 1;
    } else {
        return 0;
    }
}

// Fonction pour trier un tableau de KeyScore par score_dico
void trier_par_score_dico(KeyScore* tableauCle, int nbTotal) {
    qsort(tableauCle, nbTotal, sizeof(KeyScore), comparer_score_dico);
}

// Fonction pour charger un dictionnaire depuis un fichier
char** chargerDictionnaire(const char* filename, int* tailleDico) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier dictionnaire");
        return NULL;
    }

    int capacity = 100;
    char** dictionnaire = malloc(sizeof(char*) * capacity);
    if (dictionnaire == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(file);
        return NULL;
    }

    char buffer[256];
    *tailleDico = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (*tailleDico >= capacity) {
            capacity *= 2;
            dictionnaire = realloc(dictionnaire, sizeof(char*) * capacity);
            if (dictionnaire == NULL) {
                perror("Erreur de réallocation mémoire");
                fclose(file);
                return NULL;
            }
        }

        dictionnaire[*tailleDico] = strdup(buffer);
        if (dictionnaire[*tailleDico] == NULL) {
            perror("Erreur d'allocation mémoire pour le mot");
            fclose(file);
            return NULL;
        }
        (*tailleDico)++;
    }

    fclose(file);
    return dictionnaire;
}

int is_valid_word(const char* word, char** dictionnaire, int tailleDico) {
    for (int i = 0; i < tailleDico; i++) {
        if (strcmp(word, dictionnaire[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void break_code_c3(KeyScore* tableauCle, unsigned char* cipher, size_t nbKeys, char* dico_file, int tailleCle, size_t msg_len) {
    int tailleDico;
    char** dictionnaire = chargerDictionnaire(dico_file, &tailleDico);
    if (dictionnaire == NULL) {
        fprintf(stderr, "Impossible de charger le dictionnaire\n");
        return;
    }

    if (tailleDico == 0) {
        fprintf(stderr, "Dictionnaire vide\n");
        return;
    }

    unsigned char* decrypted_text = malloc(msg_len + 1);
    if (decrypted_text == NULL) {
        perror("Erreur d'allocation mémoire pour decrypted_text");
        for (int i = 0; i < tailleDico; i++) {
            free(dictionnaire[i]);
        }
        free(dictionnaire);
        return;
    }

    int lastProgress = -1; // Pour suivre le dernier pourcentage affiché

    for (size_t i = 0; i < nbKeys; i++) {
        unsigned char* cle = tableauCle[i].key;
        memcpy(decrypted_text, cipher, msg_len);
        xor_cipher(decrypted_text, cle, msg_len, tailleCle);
        decrypted_text[msg_len] = '\0';

        char word_buffer[256];
        int valid_word_count = 0;

        const char* str = (char*)decrypted_text;
        while (*str) {
            // Extract word manually (instead of modifying decrypted_text with strtok)
            const char* word_start = str;
            while (*str && !isspace(*str)) str++;
            size_t word_len = str - word_start;

            if (word_len < sizeof(word_buffer)) {
                strncpy(word_buffer, word_start, word_len);
                word_buffer[word_len] = '\0';

                // Convert to lowercase
                for (char* p = word_buffer; *p; ++p) *p = tolower(*p);

                if (is_valid_word(word_buffer, dictionnaire, tailleDico)) {
                    valid_word_count++;
                }
            }

            while (*str && isspace(*str)) str++; // Skip whitespace
        }

        tableauCle[i].score_dico = valid_word_count;

        // Affichage de la progression tous les 5%
        int progress = (i + 1) * 100 / nbKeys;
        if (progress % 5 == 0 && progress != lastProgress) {
            printf("\rProgress: %d%%", progress);
            fflush(stdout);
            lastProgress = progress;
        }
    }

    printf("\n"); // Fin de l'affichage de progression

    free(decrypted_text);

    for (int i = 0; i < tailleDico; i++) {
        free(dictionnaire[i]);
    }
    free(dictionnaire);
    trier_par_score_dico(tableauCle, nbKeys);
}
