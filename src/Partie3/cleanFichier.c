#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Fonction pour remplacer les caractères accentués par les non accentués
wchar_t enlever_accent(wchar_t c) {
    switch (c) {
        case L'é': return L'e';
        case L'è': return L'e';
        case L'ê': return L'e';
        case L'à': return L'a';
        case L'â': return L'a';
        case L'ä': return L'a';
        case L'î': return L'i';
        case L'ï': return L'i';
        case L'ô': return L'o';
        case L'ö': return L'o';
        case L'ù': return L'u';
        case L'û': return L'u';
        case L'ü': return L'u';
        case L'ç': return L'c';
        case L'É': return L'E';
        case L'È': return L'E';
        case L'Ê': return L'E';
        case L'À': return L'A';
        case L'Â': return L'A';
        case L'Ä': return L'A';
        case L'Î': return L'I';
        case L'Ï': return L'I';
        case L'Ô': return L'O';
        case L'Ö': return L'O';
        case L'Ù': return L'U';
        case L'Û': return L'U';
        case L'Ü': return L'U';
        case L'Ç': return L'C';
        default: return c;
    }
}

// Fonction pour lire et traiter un fichier
void traiter_fichier(const char* fichier_input, const char* fichier_output) {
    FILE* input = fopen(fichier_input, "r");
    if (input == NULL) {
        perror("Erreur lors de l'ouverture du fichier source");
        return;
    }

    FILE* output = fopen(fichier_output, "w");
    if (output == NULL) {
        perror("Erreur lors de l'ouverture du fichier de destination");
        fclose(input);
        return;
    }

    int caractere;
    int nb_remplacements = 0;

    // Lire le fichier caractère par caractère
    while ((caractere = fgetwc(input)) != WEOF) {
        wchar_t nouveau_caractere = enlever_accent((wchar_t)caractere);
        if (nouveau_caractere != (wchar_t)caractere) {
            nb_remplacements++;
        }
        fputwc(nouveau_caractere, output);
    }

    // Affichage du nombre de remplacements
    printf("Nombre de caractères remplacés : %d\n", nb_remplacements);

    fclose(input);
    fclose(output);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier_source> <fichier_destination>\n", argv[0]);
        return 1;
    }

    const char* fichier_input = argv[1];
    const char* fichier_output = argv[2];

    traiter_fichier(fichier_input, fichier_output);

    return 0;
}
