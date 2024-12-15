#include "dh_prime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

void afficher_aide() {
  printf("Usage : dh_gen_group -o <output_file>\n");
  printf("Options :\n");
  printf("  -o : spécifier le fichier de sortie pour écrire le nombre premier "
         "et le générateur\n");
  printf("  -h : afficher cette aide\n");
  exit(0);
}

int main(int argc, char *argv[]) {
  char *output_file = NULL;
  int opt;
  srand(time(NULL));

  // Lecture des options
  while ((opt = getopt(argc, argv, "o:h")) != -1) {
    switch (opt) {
    case 'o':
      output_file = optarg;
      break;
    case 'h':
      afficher_aide();
      break;
    }
  }

  if (!output_file) {
    fprintf(stderr, "Erreur : option -o (fichier de sortie) manquante.\n");
    afficher_aide();
  }

  FILE *out_fp = fopen(output_file, "w");
  if (!out_fp) {
    perror("Erreur lors de l'ouverture du fichier de sortie");
    exit(1);
  }

  long min = MIN_PRIME;
  long max = MAX_PRIME;
  int essais = 0;

  // Génération du nombre premier de Sophie Germain
  long premier = genPrimeSophieGermain(min, max, &essais);
  if (premier == -1) {
    fprintf(stderr, "Erreur : impossible de générer un nombre premier de "
                    "Sophie Germain.\n");
    fclose(out_fp);
    exit(1);
  }

  // Recherche d'un générateur
  long generateur = seek_generator(2, premier - 1);
  if (generateur == -1) {
    fprintf(stderr, "Erreur : impossible de trouver un générateur pour le "
                    "nombre premier généré.\n");
    fclose(out_fp);
    exit(2);
  }

  // Génération aléatoire des clés d'Alice et de Bob
  long cle_Alice = random_long(0, premier - 1);
  long cle_Bob = random_long(0, premier - 1);

  // Écriture dans le fichier de sortie
  fprintf(out_fp, "%ld\n", premier);
  fprintf(out_fp, "%ld\n", generateur);
  fprintf(out_fp, "%ld\n", cle_Alice);
  fprintf(out_fp, "%ld\n", cle_Bob);

  fclose(out_fp);

  printf("Nombre premier, générateur et clé publique de Alice et de Bob "
         "avec succès dans %s.\n",
         output_file);
  return 0;
}
