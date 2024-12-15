#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include "cbc.h" 
#include "xor.h"

#define DEFAULT_LOG_STREAM stderr

// Fonction pour afficher l'aide et les options du programme
void afficher_aide(FILE *output_stream) {
    fprintf(output_stream,
            "Utilisation : symcrypt -i <input_file> -o <output_file> -k <cle>/-f <key_file> -m <methode> -v <iv_file> -l <log_file>\n");
    fprintf(output_stream,
            "Options :\n"
            "  -i <input_file>          Fichier contenant le texte en clair \n"
            "  -o <output_file>         Fichier de sortie pour le texte chiffré \n"
            "  -k <cle>                 Clé pour le chiffrement/déchiffrement \n"
            "  -f <key_file>            Fichier contenant la clé \n"
            "  -m <methode>             Méthode (xor, cbc-crypt, cbc-decrypt, mask)\n"
            "  -v <iv_file>             Fichier du vecteur d'initialisation (obligatoire pour CBC)\n"
            "  -l <log_file>            Fichier de log (optionnel, par défaut stderr)\n"
            "  -h                       Afficher ce message d'aide\n");
}

// Fonction pour lire le contenu d'un fichier
void read_file(const char *nom_fichier, unsigned char **contenu, size_t *len_file) {
    FILE *fichier = fopen(nom_fichier, "rb");
    if (!fichier) {
        fprintf(stderr, "Erreur lors d'ouverture du fichier pour lire '%s' : %s\n", nom_fichier, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //dDéterminer la taille du fichier
    fseek(fichier, 0, SEEK_END);
    *len_file = ftell(fichier);
    rewind(fichier);
    
    // allouer de la mémoire pour le stocker le contenu du fichier
    *contenu = malloc(*len_file);
    if (!*contenu) {
        perror("Erreur lors d'allocation de mémoire");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }
    
    // lire le fichier dans le tampon
    fread(*contenu, 1, *len_file, fichier);
    fclose(fichier);
}

// Fonction pour écrire le contenu dans un fichier
void write_file(const char *nom_fichier, const unsigned char *contenu, size_t len_file) {
    FILE *fichier = fopen(nom_fichier, "wb");
    if (!fichier) {
        perror("Erreur lors d'ouverture du fichier pour ecrire");
        exit(EXIT_FAILURE);
    }
    
    //ecrire le contenu dqns le fichier
    fwrite(contenu, 1, len_file, fichier);
    fclose(fichier);
}

int main(int argc, char *argv[]) {
    const char *input_file = NULL, *output_file = NULL, *key_file = NULL, *key = NULL,  *methode = NULL, *iv_file = NULL, *log_file = NULL;
    
    FILE *log_flux = DEFAULT_LOG_STREAM;

    unsigned char *vecteur_init = NULL, *cle = NULL;
    size_t len_iv = 0, key_len = 0;

    // analyse des options passées en ligne de commande
    int opt;
    while ((opt = getopt(argc, argv, "i:o:k:f:m:v:l:h")) != -1) {
        switch (opt) {
            case 'i': input_file = optarg; break;  
            case 'o': output_file = optarg; break; 
            case 'k': key = optarg; break; 
            case 'f': key_file = optarg; break;
            case 'm': methode = optarg; break;
            case 'v': iv_file = optarg; break;
            case 'l': log_file = optarg; break;
            case 'h': afficher_aide(stdout); return 0;
            default: afficher_aide(stderr); return EXIT_FAILURE;
        }
    }

    if (log_file) {
        log_flux = fopen(log_file, "w");
        if (!log_flux) {
            fprintf(stderr, "Erreur lors d'ouverture du fichier de log '%s'\n", log_file);
            return EXIT_FAILURE;
        }
    }

    if (log_flux) fprintf(log_flux, "Debut de chiffrement.\n");

    // vérification des arguments requis
    if (!input_file || !output_file || (!key && !key_file) || (!key && !key_file && strcmp(methode, "mask") != 0) 
        || (!iv_file && strcmp(methode, "cbc-crypt") == 0) || (!iv_file && strcmp(methode, "cbc-decrypt") == 0)|| !methode) {
        fprintf(stderr, "Arguments manquants.\n");
        if (log_flux) fprintf(log_flux, "Arguments manquants.\n");
        afficher_aide(stdout);
        return EXIT_FAILURE;
    }

    // lecture du fichier d'entrée
    unsigned char *input = NULL;
    size_t message_len = 0;
    read_file(input_file, &input, &message_len);

    // allocation de mémoire pour la sortie
    unsigned char *output = malloc(message_len + TAILLE_BLOC);
    if (!output) {
        perror("Erreur lors d'allocation de mémoire pour le output");
        if (log_flux) fprintf(log_flux, "Erreur lors d'allocation de mémoire pour le output\n");
        free(input);
        free(vecteur_init);
        return EXIT_FAILURE;
    }

    // lecture de la clé si fournie dans un fichier
    if (key_file) {
        read_file(key_file, &cle, &key_len);
        if (log_flux) fprintf(log_flux, "Clé lue depuis le fichier '%s'\n", key_file);
    } else {
        key_len = strlen(key);
        cle = (unsigned char *)key;
        if (log_flux) fprintf(log_flux, "Clé indiqué sur la ligne de commande\n");
    }

    // lecture du vecteur d'initialisation IV pour le CBC
    if ((strcmp(methode, "cbc-crypt") == 0 || strcmp(methode, "cbc-decrypt") == 0) && iv_file) {
        size_t len_iv = 0;
        read_file(iv_file, &vecteur_init, &len_iv);
        if (len_iv != TAILLE_BLOC) {
            fprintf(stderr, "Erreur: IV doit faire %d octets à la place de %d.\n", TAILLE_BLOC, len_iv);
            if (log_flux) fprintf(log_flux, "Erreur: IV doit faire %d octet à la place de %d.\n", TAILLE_BLOC, len_iv);
            free(vecteur_init);
            return EXIT_FAILURE;
        }
    }
    
    // exécution de la méthode choisie
    if (strcmp(methode, "xor") == 0) {
        // XOR
        size_t key_len = strlen((const char *)cle);
        xor_chiffrement(input, cle, message_len, key_len, output);
        if (log_flux) fprintf(log_flux, "Chiffrement XOR effectué avec succes.\n");
    } else if (strcmp(methode, "cbc-crypt") == 0) {
        // chiffrement CBC
        cbc_chiffrement(input, message_len, cle, vecteur_init, output);
        if (log_flux) fprintf(log_flux, "Chiffrement CBC effectué avec succes.\n");
    } else if (strcmp(methode, "cbc-decrypt") == 0) {
        // dechiffrement CBC
        cbc_dechiffrement(input, message_len, cle, vecteur_init, output);
        if (log_flux) fprintf(log_flux, "Déchiffrement CBC effectué avec succes.\n");
    } else if (strcmp(methode, "mask") == 0) {
        // Générer une clé aléatoire si mask
        cle = malloc(message_len + 1);
        if (!cle) {
            perror("Erreur d'allocation de mémoire pour la clé générée");
            free(input);
            free(output);
            return EXIT_FAILURE;
        }

        //générer le clé 
        gen_key(cle, message_len);
        // Calcul de la longueur de la clé
        size_t key_len = strlen((const char *)cle);
        xor_chiffrement(input, cle, message_len, key_len, output);

        if (log_flux) fprintf(log_flux, "Chiffrement XOR effectué.\n");
        save_key_to_file("used_keys.txt", cle); 

        if (log_flux) fprintf(log_flux, "Écriture dans le fichier '%s' réussie, taille : %lu octets\n", key_file, (unsigned long)message_len);
    } else {
        fprintf(stderr, "Méthode inconnue.\n");
        if (log_flux) fprintf(log_flux, "Méthode inconnue '%s'.\n", methode);
        free(input);
        free(output);
        free(vecteur_init);
        if (key_file) free(cle);
        return EXIT_FAILURE;
    }

    // écriture du résultat dans le fichier de sortie
    write_file(output_file, output, message_len);

    if (log_flux) fprintf(log_flux, "Processus terminé avec succès.\n");
    
    // libération des ressources
    free(input);
    free(output);
    free(vecteur_init);
    if (key_file) free(cle);

    if (log_flux != stderr) fclose(log_flux);

    return EXIT_SUCCESS;
}
