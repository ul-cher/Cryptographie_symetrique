#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Fonction pour chiffrer et déchiffrer avec XOR
void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len) {
    for (size_t i = 0; i < msg_len; i++) {
        msg[i] ^= key[i];
    }
}

// Fonction pour générer une clé aléatoire de la même longueur que le message
void gen_random_key(unsigned char *key, size_t length) {
    srand(time(NULL));
    for (size_t i = 0; i < length; i++) {
        key[i] = rand() % 256;  // Génère un entier aléatoire sur 8 bits (0 à 255)
    }
}

// Vérifie si la clé est unique en comparant avec les clés du fichier
int is_unique_key(unsigned char *key, size_t key_len) {
    FILE *file = fopen("used_keys.txt", "r"); 
    if (!file) {
        return 1; // Fichier n'existe pas encore, donc clé est unique
    }

    char existing_key[256]; // Buffer pour les clés
    while (fgets(existing_key, sizeof(existing_key), file)) {
        existing_key[strcspn(existing_key, "\n")] = 0; //supprimer newline 
        
        // Comparer existing_key avec le nouveau clé en format hexadécimal
        char new_key[256];
        for (size_t i = 0; i < key_len; i++) {
            sprintf(new_key + i * 3, "%02x ", key[i]); // Convertir au format hex
        }
        new_key[key_len * 3 - 1] = '\0';
        if (strcmp(existing_key, new_key) == 0) {
            fclose(file);
            perror("Le clé exists déjà");
            exit(1);
        }
    }
    fclose(file);
    return 1; // Le clé est unique
}

// Génère une clé unique
void generate_unique_key(unsigned char *key, size_t key_len) {
    do {
        gen_random_key(key, key_len);
    } while (!is_unique_key(key, key_len));
}

void save_key(unsigned char *key, size_t length) {
    FILE *file = fopen("used_keys.txt", "a"); 
    if (file == NULL) {
        perror("Error opening file to save key");
        exit(1);
    }

    char hex_key[256]; // Buffer pour representation hex
    for (size_t i = 0; i < length; i++) {
        sprintf(hex_key + i * 3, "%02x ", key[i]); 
    }
    hex_key[length * 3 - 1] = '\0'; 

    fprintf(file, "%s\n", hex_key); // Ecrire le clé dans le fichier
    fclose(file);
}

int main() {
   srand((unsigned int) time(NULL)); // Initialise le générateur aléatoire

    // Entrer un message depuis l'utilisateur
    unsigned char msg[256];
    printf("Entrez le message à chiffrer : ");
    fgets((char*)msg, sizeof(msg), stdin);
    size_t msg_len = strlen((const char*)msg);
    if (msg[msg_len - 1] == '\n') msg[--msg_len] = '\0'; // Supprime le '\n' final

    // Générer une clé unique de la même longueur que le message
    unsigned char *key = malloc(msg_len); 
    if (key == NULL) {
        perror("Échec de l'allocation de mémoire pour la clé");
        exit(1);
    }
    generate_unique_key(key, msg_len); 
    save_key(key, msg_len);

    printf("Message avant chiffrement : %s\n", msg);

    // Chiffrement
    xor_cipher(msg, key, msg_len);

    printf("Message chiffré : ");
    for (size_t i = 0; i < msg_len; i++) {
        printf("%02x ", msg[i]);
    }
    printf("\n");

    // Déchiffrement
    xor_cipher(msg, key, msg_len);
    printf("Message après déchiffrement : %s\n", msg);

    return 0;
}
