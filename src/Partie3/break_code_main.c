#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "break_code.h"

Generateur* genCle;

int ecrireTableauKeyScore(const char* filename, KeyScore* tableau, int taille) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    fprintf(file, "Clé (Texte)            | Score Fréquence | Score Dictionnaire\n");
    fprintf(file, "-----------------------------------------------------------\n");

    for (int i = 0; i < taille; i++) {
        fprintf(file, "%-20s | %-17.5f | %-19d\n", 
            tableau[i].key, 
            tableau[i].score_freq, 
            tableau[i].score_dico);
    }

    fclose(file);
    return 0;
}

int write_to_file(FILE* file, unsigned char* data, size_t data_size) {
    if (file == NULL) {
        perror("File not open");
        return -1;
    }
    size_t written = fwrite(data, 1, data_size, file);
    if (written != data_size) {
        perror("Error writing to file");
        return -1;
    }

    return 0;
}

// Fonction pour lire le fichier par blocs et remplacer les accents
unsigned char* read_file(const char* cipher_file, size_t* msg_len) {
    FILE* file = fopen(cipher_file, "rb");
    if (file == NULL) {
        perror("Error opening cipher file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *msg_len = file_size;
    unsigned char* buffer = (unsigned char*)malloc(file_size);
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return NULL;
    }

    size_t total_read = 0;
    size_t read_size;
    while ((read_size = fread(buffer + total_read, 1, file_size - total_read, file)) > 0) {
        total_read += read_size;
    }

    fclose(file);
    if (total_read != *msg_len) {
        perror("Error reading entire file");
        free(buffer);
        return NULL;
    }

    return buffer;
}

// Fonction modifiée pour afficher les clés
void afficherTraceC1() {
    printf("NbCle = %llu\n", genCle->nbTotal);
    for (int i = 0; i < genCle->tailleCle; i++) {
        printf("Cle[%d] = %s\n", i, genCle->clesValide[i]);
    }
}

// Fonction modifiée pour écrire les clés dans un fichier
void ecrireCleC1(FILE* fichier) {
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    fprintf(fichier, "NbTotal de cle = %llu\n", genCle->nbTotal);
    
    // Removed unnecessary allocation for cle, using the existing one from genCle->clesValide
    unsigned char* cle = NULL;

    for (int i = 0; i < genCle->nbTotal; i++) {
        cle = genCle->clesValide[i];
        fprintf(fichier, "cle[%d] = \"", i);
        for (int j = 0; j < genCle->tailleCle; j++) {
            fprintf(fichier, "%c", cle[j]);
        }
        fprintf(fichier, "\"\n");
    }
}

// Fonction pour afficher l'utilisation du programme
void print_usage(const char* program_name) {
    fprintf(stderr, "Usage: %s -i <cipher_file> -m <method> -k <key_length> [-d <dictionary_file>] [-l <log_file>] [-h]\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "  -i, --input            Input cipher file (required)\n");
    fprintf(stderr, "  -m, --method           Method for breaking the code (c1 or all) (required)\n");
    fprintf(stderr, "  -k, --key-length       Length of the key (required)\n");
    fprintf(stderr, "  -d, --dictionary       Dictionary file for method 'all' (optional)\n");
    fprintf(stderr, "  -l, --log              Log file for results (optional)\n");
    fprintf(stderr, "  -h, --help             Show this help message\n");
}

// Fonction principale
int main(int argc, char* argv[]) {
    int key_length = 0;
    char* cipher_file = NULL;
    char* method = NULL;
    char* dictionary_file = NULL;
    char* log_file = NULL;

    struct option long_options[] = {
        {"input", required_argument, NULL, 'i'}, 
        {"method", required_argument, NULL, 'm'}, 
        {"key-length", required_argument, NULL, 'k'},
        {"dictionary", required_argument, NULL, 'd'},
        {"log", required_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;
    while ((c = getopt_long(argc, argv, "i:m:k:d:l:h", long_options, &option_index)) != -1) {
        switch (c) {
            case 'i':
                cipher_file = optarg;
                break;
            case 'm':
                method = optarg;
                break;
            case 'k':
                key_length = atoi(optarg);
                break;
            case 'd':
                dictionary_file = optarg;
                break;
            case 'l':
                log_file = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    // Validate inputs
    if (key_length <= 0) {
        fprintf(stderr, "Error: key-length must be a positive integer.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (method == NULL || (strcmp(method, "c1") != 0 && strcmp(method, "all") != 0)) {
        fprintf(stderr, "Error: Invalid method. Use 'c1' or 'all'.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (!cipher_file) {
        fprintf(stderr, "Error: Input cipher file is required.\n");
        print_usage(argv[0]);
        return 1;
    }

    FILE* log = fopen(log_file, "a");
    if (log == NULL) {
        fprintf(stderr, "Error: Unable to open log file %s for writing. Check file permissions.\n", log_file);
        return 1;
    }

    // Processing cipher text
    size_t msg_len;
    unsigned char* messageCode = read_file(cipher_file, &msg_len);
    if (messageCode == NULL) {
        fclose(log);
        return 1;
    }

    if (strcmp(method, "c1") == 0) {
        genCle = break_code_c1(messageCode, msg_len, key_length);
        afficherTraceC1();
    } else if (strcmp(method, "all") == 0) {
        genCle = break_code_c1(messageCode, msg_len, key_length);
        afficherTraceC1();
        
        KeyScore* tableauCle = malloc(sizeof(KeyScore) * genCle->nbTotal);
        if (tableauCle == NULL) {
            perror("Memory allocation error");
            free(messageCode);
            fclose(log);
            return 1;
        }
        
        break_code_c2(tableauCle, messageCode, genCle, msg_len);
        printf("break_code_c2 OK\n");
        break_code_c3(tableauCle, messageCode, genCle->nbTotal, dictionary_file, key_length, msg_len);
        printf("break_code_c3 OK\n");
        ecrireTableauKeyScore(log_file, tableauCle, genCle->nbTotal);
        libererTableau(tableauCle, genCle->nbTotal);
    }

    free(messageCode);
    fclose(log);
    libererGenerateur(genCle);
    return 0;
}
