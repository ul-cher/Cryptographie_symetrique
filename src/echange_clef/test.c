#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_dh_gen_group() {
  printf("=== Début des tests ===\n");

  // Test 1 : Vérification de l'affichage de l'aide
  printf("Test 1 : Affichage de l'aide\n");
  int ret = system("./dh_gen_group -h > test_help.txt");
  assert(ret == 0);
  FILE *help_fp = fopen("test_help.txt", "r");
  assert(help_fp != NULL);
  fclose(help_fp);
  printf("Test 1 réussi.\n");

  // Test 2 : Génération avec un fichier de sortie valide
  printf("Test 2 : Génération avec un fichier valide\n");
  ret = system("./dh_gen_group -o test_output.txt");
  assert(ret == 0);
  FILE *output_fp = fopen("test_output.txt", "r");
  assert(output_fp != NULL);
  fclose(output_fp);
  printf("Test 2 réussi.\n");

  // Test 3 : Absence d'option -o
  printf("Test 3 : Options manquantes\n");
  ret = system("./dh_gen_group > test_error.txt 2>&1");
  assert(ret == 0);
  printf("Test 3 réussi.\n");

  remove("test_help.txt");
  remove("test_output.txt");
  remove("test_error.txt");

  printf("=== Tous les tests ont réussi ! ===\n");
}

int main() {
  test_dh_gen_group();
  return 0;
}
