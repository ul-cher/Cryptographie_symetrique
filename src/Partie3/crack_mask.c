#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024

// Fonction pour lire un fichier en binaire
unsigned char *read_file(const char *file_path, long *file_size) {
    FILE *file = fopen(file_path, "rb");  // Lire en mode binaire
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Calculer la taille du fichier
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allouer de la mémoire pour stocker le contenu du fichier
    unsigned char *content = (unsigned char *)malloc(*file_size);
    if (content == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Lire le fichier
    fread(content, 1, *file_size, file);
    fclose(file);
    return content;
}

// Fonction pour écrire un fichier en binaire
void write_file(const char *file_path, const unsigned char *content, long size) {
    FILE *file = fopen(file_path, "wb");  // Écrire en mode binaire
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fwrite(content, 1, size, file);
    fclose(file);
}

// Fonction pour calculer la clé en effectuant un XOR entre le texte chiffré et le texte clair
unsigned char *calculate_key(const unsigned char *ciphertext, const unsigned char *plaintext, long len) {
    unsigned char *key = (unsigned char *)malloc(len);
    if (key == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    for (long i = 0; i < len; i++) {
        key[i] = ciphertext[i] ^ plaintext[i];  // XOR entre chaque caractère
    }
    return key;
}

// Fonction pour effectuer un XOR entre le texte chiffré et la clé pour déchiffrer
unsigned char *xor_decrypt(const unsigned char *ciphertext, const unsigned char *key, long len) {
    unsigned char *decrypted_text = (unsigned char *)malloc(len);
    if (decrypted_text == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    for (long i = 0; i < len; i++) {
        decrypted_text[i] = ciphertext[i] ^ key[i];  // XOR entre chaque caractère
    }
    return decrypted_text;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <chif1.txt> <chif2.txt> <clair1.txt> <clair.txt>\n", argv[0]);
        return 1;
    }

    long size_chif1, size_clair1, size_chif2;

    // Lire les fichiers en binaire
    unsigned char *chif1 = read_file(argv[1], &size_chif1);  // Texte chiffré 1
    unsigned char *chif2 = read_file(argv[2], &size_chif2);  // Texte chiffré 2
    unsigned char *clair1 = read_file(argv[3], &size_clair1); // Texte clair 1

    // Vérifier que les tailles des fichiers correspondent
    if (size_chif1 != size_clair1) {
        fprintf(stderr, "Les fichiers chiffré et clair ne sont pas de la même taille !\n");
        return 1;
    }

    // Calculer la clé en effectuant un XOR entre le texte chiffré 1 et le texte clair 1
    unsigned char *key = calculate_key(chif1, clair1, size_clair1);

    // Vérifier que le texte chiffré 2 a la même taille que le texte chiffré 1
    if (size_chif2 != size_chif1) {
        fprintf(stderr, "Le fichier chiffré 2 n'a pas la même taille que le fichier chiffré 1 !\n");
        return 1;
    }

    // Déchiffrer le texte chiffré 2 avec la clé calculée
    unsigned char *decrypted_text = xor_decrypt(chif2, key, size_chif2);

    // Sauvegarder le texte déchiffré dans un fichier
    write_file(argv[4], decrypted_text, size_chif2);

    // Libérer la mémoire allouée
    free(chif1);
    free(chif2);
    free(clair1);
    free(key);
    free(decrypted_text);
    printf("Resultat ecris dans le fichier : %s\n", argv[4]);
    return 0;
}
