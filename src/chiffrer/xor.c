#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Fonction pour générer une clé alphanumérique aléatoire
void gen_key(unsigned char *key, size_t length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t charset_size = sizeof(charset) - 1;

    //init le générateur de nombres aléatoires
    srand(time(NULL));

    for (size_t i = 0; i < length; i++) {
        key[i] = charset[rand() % charset_size];
    }
    key[length] = '\0';  //terminer la chaîne avec un caractère nul
}


// Fonction pour chiffrer et déchiffrer avec XOR
void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len, size_t key_len) {
    for (size_t i = 0; i < msg_len; i++) {
        msg[i] ^= key[i % key_len];
    }
}

void save_key_to_file(const char *filename, unsigned char *key){
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", key); 
        fclose(file);
    } else {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier pour sauvegarder la clé.\n");
    }
}
 
// int main() {
//     // ex de message et de clé
//     //unsigned char msg[] = "Les carottes sont cuites";
//     unsigned char msg[1024];
//     printf("Tapez votre message: ");
//     fgets(msg, sizeof(msg), stdin);

//     size_t key_len = 16;

//     //allouer dynamiquement un tableau pour la clé, +1 pour le caractère de fin '\0'
//     unsigned char *key = (unsigned char *)malloc((key_len + 1) * sizeof(unsigned char));

//     if (key == NULL) {
//         printf("Échec de l'allocation de mémoire.\n");
//         return 1;
//     }

//     // Générer une clé aléatoire
//     gen_key(key, key_len);
//     printf("Generated clef : %s\n", key);

//     free(key);

//     // Calcul de la longueur du message et de la clé
//     size_t msg_len = strlen((const char*)msg);

//     printf("Message avant chiffrement : %s", msg); //msg
//     xor_cipher(msg, key, msg_len, key_len); // Chiffrement

//     printf("Message chiffre : ");
//     for (size_t i = 0; i < msg_len; i++) {
//         printf("%02x ", msg[i]); 
//     }
//     printf("\n");

//     xor_cipher(msg, key, msg_len, key_len); // Déchiffrement

//     printf("Message apres dechiffrement : %s", msg);

//     return 0;
// }
