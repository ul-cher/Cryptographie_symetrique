#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Définir une clé statique
#define CLE "Cle" // La clé utilisée pour le chiffrement

// Fonction pour chiffrer et déchiffrer avec XOR
void xor_cipher(unsigned char *msg, const char *key, size_t msg_len, size_t key_len) {
    for (size_t i = 0; i < msg_len; i++) {
        msg[i] ^= key[i % key_len];
    }
}

int xor_file(const char *source_filename, const char *dest_filename) {
    // Ouvre le fichier source en mode binaire pour lecture
    FILE *source_file = fopen(source_filename, "rb");
    if (source_file == NULL) {
        perror("Erreur d'ouverture du fichier source");
        return -1;
    }

    // Détermine la taille du fichier source
    fseek(source_file, 0, SEEK_END);
    size_t file_size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    // Alloue un tampon pour stocker le contenu du fichier source
    unsigned char *buffer = malloc(file_size);
    if (buffer == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(source_file);
        return -1;
    }

    // Lit le contenu du fichier source
    size_t read_size = fread(buffer, 1, file_size, source_file);
    if (read_size != file_size) {
        perror("Erreur de lecture du fichier source");
        free(buffer);
        fclose(source_file);
        return -1;
    }

    // Taille de la clé
    size_t key_len = strlen(CLE);

    // Affiche la clé utilisée (pour vérification uniquement)
    printf("Clé utilisée: %s\n", CLE);

    // Applique le XOR avec la clé statique
    xor_cipher(buffer, CLE, file_size, key_len);

    // Ouvre le fichier de destination en mode binaire pour écriture
    FILE *dest_file = fopen(dest_filename, "wb");
    if (dest_file == NULL) {
        perror("Erreur d'ouverture du fichier destination");
        free(buffer);
        fclose(source_file);
        return -1;
    }

    // Écrit le contenu chiffré dans le fichier destination
    size_t written_size = fwrite(buffer, 1, file_size, dest_file);
    if (written_size != file_size) {
        perror("Erreur d'écriture dans le fichier destination");
        free(buffer);
        fclose(source_file);
        fclose(dest_file);
        return -1;
    }

    // Libère la mémoire et ferme les fichiers
    free(buffer);
    fclose(source_file);
    fclose(dest_file);

    return 0;
}

int main(int argc, char *argv[]) {
    // Vérifie que les bons arguments sont fournis
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier_source> <fichier_destination>\n", argv[0]);
        return 1;
    }

    // Appelle la fonction pour chiffrer le fichier source et écrire dans le fichier destination
    if (xor_file(argv[1], argv[2]) != 0) {
        fprintf(stderr, "Erreur lors du traitement des fichiers.\n");
        return 1;
    }

    printf("Fichier chiffré avec succès.\n");
    return 0;
}
