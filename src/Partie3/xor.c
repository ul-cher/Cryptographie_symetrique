#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Fonction pour générer une clé alphanumérique aléatoire
void gen_key(unsigned char *key, size_t length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t charset_size = sizeof(charset) - 1;

    // Init le générateur de nombres aléatoires
    srand(time(NULL));

    for (size_t i = 0; i < length; i++) {
        key[i] = charset[rand() % charset_size];
    }
    key[length] = '\0';  // Terminer la chaîne avec un caractère nul
}

// Fonction pour chiffrer et déchiffrer avec XOR
void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len, size_t key_len) {
    for (size_t i = 0; i < msg_len; i++) {
        msg[i] ^= key[i % key_len];
    }
}

int xor_file(const char *source_filename, const char *dest_filename, unsigned char *key, size_t key_len) {
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

    // Applique le XOR avec la clé
    xor_cipher(buffer, key, file_size, key_len);

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
