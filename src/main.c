#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "chiffrer/cbc.h" // Inclure les implémentations CBC
#include "chiffrer/xor.h" // Inclure les implémentations XOR

#define MAX_KEY_LENGTH 256
#define LOG_FILE "log_file.txt"
#define KEY_FILE "keys.txt"

FILE *log_file, *key_file;

void log_message(const char *message) {
    if (log_file) {
        fprintf(log_file, "%s\n", message);
    }
}

void afficher_aide() {
    printf("Commandes disponibles :\n");
    printf("  help : Affiche la liste des commandes\n");
    printf("  list-keys : Affiche la liste des clefs générées et leur état\n");
    printf("  gen-key <n> : Génère une clef de longueur n\n");
    printf("  del-key <key> : Supprime la clef spécifiée\n");
    printf("  encrypt <in> <out> <key> <method> [<vecteur_initialisation>] : Chiffre un fichier\n");
    printf("  decrypt <in> <out> <key> <method> [<vecteur_initialisation>] : Déchiffre un fichier\n");
    printf("  crack <in> <out> <length> <dico> : Tente de craquer le chiffrement\n");
    printf("  quit : Quitte le programme\n");
}

void lister_cles() {
    FILE *file = fopen(KEY_FILE, "r"); 
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier des clés");
        return; 
    }

    //verifier si le fichier est vide
    fseek(file,0,SEEK_END);
    if(ftell(file)==0) {
        printf("Aucune clé trouvée. \n"); 
        return;
    }
    rewind(file);

    printf("Clefs disponibles: \n");
    char buffer[MAX_KEY_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprimer le saut de ligne
        printf("  %s\n", buffer);
    }
    fclose(file);
}

void generer_cle(int longueur) {
    if (longueur <= 0 || longueur >= MAX_KEY_LENGTH) {
        printf("Longueur de clef invalide.\n");
        return;
    }
    //allouer dynamiquement un tableau pour la clé, +1 pour le caractère de fin '\0'
    unsigned char *key = (unsigned char *)malloc((longueur + 1) * sizeof(unsigned char));
    if (key == NULL) {
        printf("Échec de l'allocation de mémoire.\n");
        return;
    }
    // Générer une clé aléatoire
    gen_key(key, longueur);
    printf("Clef générée: %s\n", key);
    save_key_to_file(KEY_FILE, key);
    free(key);
    log_message("Clef générée avec succès.");
}

void supprimer_cle(const char *cle) {
    FILE *file = fopen(KEY_FILE, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier des clés");
        return;
    }
    char temp_file_name[1024];

    // creer le fichier temporaire
    snprintf(temp_file_name, sizeof(temp_file_name), "%s.tmp", KEY_FILE);

    FILE *temp_file = fopen(temp_file_name, "w");
    if (!temp_file) {
        perror("Erreur lors de l'ouverture du fichier temporaire");
        fclose(file);
        return;
    }

    char buffer[MAX_KEY_LENGTH];
    int key_found = 0;
    //recopier les clés dans le fichier temporaire sauf le "cle"
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprimer le saut de ligne
        if (strcmp(buffer, cle) != 0) {
            fprintf(temp_file, "%s\n", buffer);
        } else {
            key_found = 1;
        }
    }
    fclose(file);
    fclose(temp_file);

    int status1 = remove(KEY_FILE);
    int status2 = rename(temp_file_name, KEY_FILE);

    if (status1 != 0) {
        perror("Erreur lors de la suppression du fichier original");
    }
    if (status2 != 0) {
        perror("Erreur lors du renommage du fichier temporaire");
    }

    if (key_found) {
        printf("Clef supprimée avec succès : %s\n", cle);
    } else {
        printf("Clef introuvable : %s\n", cle);
        remove(temp_file_name);
    }
}

void chiffrer(const char *in, const char *out, const char *cle, const char *method, const char *iv) {
    printf("Chiffrement : %s\n", in);
    char commande[512];

    // path vers `sym_crypt`
    const char *path_to_sym_crypt = "chiffrer\\sym_crypt.exe"; ;

    if (iv) {
        snprintf(commande, sizeof(commande), 
                 "%s -i %s -o %s -k %s -m %s -v %s -l %s", 
                 path_to_sym_crypt, in, out, cle, method, iv, LOG_FILE);
    } else {
        snprintf(commande, sizeof(commande), 
                 "%s -i %s -o %s -k %s -m %s -l %s", 
                 path_to_sym_crypt, in, out, cle, method, LOG_FILE);
    }

    printf("Exécution de la commande : %s\n", commande);

    // execution
    int ret = system(commande);
    if (ret == -1) {
        perror("Erreur lors de l'exécution de sym_crypt");
        log_message("Erreur lors de l'exécution de sym_crypt.");
    } else {
        printf("Chiffrement terminé avec succès.\n");
        log_message("Chiffrement effectué avec succès.");
    }
}


void dechiffrer(const char *in, const char *out, const char *cle, const char *method, const char *iv) {
    printf("Déchiffrement : %s\n", in);
    char commande[512];

    // path vers `sym_crypt`
    const char *path_to_sym_crypt = "chiffrer\\sym_crypt.exe"; ;

    if (iv) {
        snprintf(commande, sizeof(commande), 
                 "%s -i %s -o %s -k %s -m %s -v %s -l %s", 
                 path_to_sym_crypt, in, out, cle, method, iv, LOG_FILE);
    } else {
        snprintf(commande, sizeof(commande), 
                 "%s -i %s -o %s -k %s -m %s -l %s", 
                 path_to_sym_crypt, in, out, cle, method, LOG_FILE);
    }

    printf("Exécution de la commande : %s\n", commande);

    // execution
    int ret = system(commande);
    if (ret == -1) {
        perror("Erreur lors de l'exécution de sym_crypt");
        log_message("Erreur lors de l'exécution de sym_crypt.");
    } else {
        printf("Déchiffrement terminé avec succès.\n");
        log_message("Déchiffrement effectué avec succès.");
    }
}

void cracker(const char *in, const char *out, int length, const char *dico) {
    printf("Tentative de craquage : %s\n", in);
    log_message("Crackage lancé.");
    // Implémentation du craquage ici
}

int main() {
    log_file = fopen(LOG_FILE, "wa");
    if (!log_file) {
        perror("Erreur d'ouverture du fichier de log");
        return EXIT_FAILURE;
    }

    char commande[256];
    printf("Tapez 'help' pour voir la liste des commandes.\n");

    while (1) {
        printf("> ");
        if (!fgets(commande, sizeof(commande), stdin)) {
            break;
        }

        size_t len = strlen(commande);
        if (len > 0 && commande[len - 1] == '\n') {
            commande[len - 1] = '\0';
        }

        char *cmd = strtok(commande, " ");
        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0) {
            afficher_aide();
        } else if (strcmp(cmd, "list-keys") == 0) {
            lister_cles();
        } else if (strcmp(cmd, "gen-key") == 0) {
            char *arg = strtok(NULL, " \n");
            if (arg) {
                generer_cle(atoi(arg));
            } else {
                printf("Usage: gen-key <n>\n");
            }
        } else if (strcmp(cmd, "del-key") == 0) {
            char *arg = strtok(NULL, " \n");
            if (arg) {
                supprimer_cle(arg);
            } else {
                printf("Usage: del-key <key>\n");
            }
        } else if (strcmp(cmd, "encrypt") == 0) {
            char *in = strtok(NULL, " ");
            char *out = strtok(NULL, " ");
            char *cle = strtok(NULL, " ");
            char *method = strtok(NULL, " ");
            char *iv = strtok(NULL, " \n");
            if (in && out && cle && method) {
                chiffrer(in, out, cle, method, iv);
            } else {
                printf("Usage: encrypt <in> <out> <key> <method> [<iv>]\n");
            }
        } else if (strcmp(cmd, "decrypt") == 0) {
            char *in = strtok(NULL, " ");
            char *out = strtok(NULL, " ");
            char *cle = strtok(NULL, " ");
            char *method = strtok(NULL, " ");
            char *iv = strtok(NULL, " \n");
            if (in && out && cle && method) {
                dechiffrer(in, out, cle, method, iv);
            } else {
                printf("Usage: decrypt <in> <out> <key> <method> [<iv>]\n");
            }
        } else if (strcmp(cmd, "crack") == 0) {
            char *in = strtok(NULL, " ");
            char *out = strtok(NULL, " ");
            char *length = strtok(NULL, " ");
            char *dico = strtok(NULL, " \n");
            if (in && out && length && dico) {
                cracker(in, out, atoi(length), dico);
            } else {
                printf("Usage: crack <in> <out> <length> <dico>\n");
            }
        } else if (strcmp(cmd, "quit") == 0) {
            break;
        } else {
            printf("Commande inconnue : %s\n", cmd);
        }
    }

    fclose(log_file);
    return EXIT_SUCCESS;
}
