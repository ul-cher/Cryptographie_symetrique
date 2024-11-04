#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void erreur(char *message, int code);
void gen_key(unsigned char *key, size_t length);
void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len,
                size_t key_len);
void test_gen_key();
void test_xor_cipher();

int main() {
  unsigned char msg[1024];

  srand(time(NULL));

  // taille de la clé
  size_t key_len;
  printf("Entrez la taille de la clé : ");
  scanf("%zu", &key_len);

  unsigned char *key =
      (unsigned char *)malloc((key_len + 1) * sizeof(unsigned char));
  if (key == NULL) {
    erreur("Erreur d'allocation mémoire pour la clé", 1);
  }

  // Vider le buffer après scanf
  getchar();

  printf("Tapez votre message : ");
  if (fgets((char *)msg, sizeof(msg), stdin) == NULL) {
    erreur("Erreur lors de la lecture avec fgets", 3);
  }

  // Générer une clé aléatoire
  gen_key(key, key_len);
  printf("Clé générée : %s\n", key);

  // Calcul de la longueur du message
  size_t msg_len = strlen((const char *)msg);

  // Chiffrer le message
  printf("Message avant chiffrement : %s", msg);
  xor_cipher(msg, key, msg_len, key_len);

  // Afficher le message chiffré en hexadécimal
  printf("Message chiffré : ");
  for (size_t i = 0; i < msg_len; i++) {
    printf("%02x ", msg[i]);
  }
  printf("\n");

  // Déchiffrer le message
  xor_cipher(msg, key, msg_len, key_len);
  printf("Message après déchiffrement : %s\n", msg);

  free(key);

  // Exécuter les tests
  test_gen_key();
  test_xor_cipher();

  return 0;
}

// Fonction de gestion d'erreurs
void erreur(char *message, int code) {
  perror(message);
  exit(code);
}

// Fonction pour générer une clé alphanumérique aléatoire
void gen_key(unsigned char *key, size_t length) {
  const char charset[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  size_t charset_size = sizeof(charset) - 1;

  for (size_t i = 0; i < length; i++) {
    key[i] = charset[rand() % charset_size];
  }
  key[length] = '\0';
}

// Fonction pour chiffrer et déchiffrer avec XOR
void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len,
                size_t key_len) {
  for (size_t i = 0; i < msg_len; i++) {
    msg[i] ^= key[i % key_len];
  }
}

// Fonction de test pour gen_key
void test_gen_key() {
  printf("-------Début des tests de la fonction gen_key-------\n");

  // Test 1 : Vérification de la longueur et des caractères alphanumériques pour
  // une clé de 16 caractères
  size_t key_len = 16;
  unsigned char key[17];

  gen_key(key, key_len);

  // Vérification 1.1 : La clé doit avoir la longueur attendue
  assert(strlen((const char *)key) == key_len);

  // Vérification 1.2 : Tous les caractères doivent être alphanumériques
  for (size_t i = 0; i < key_len; i++) {
    assert(isalnum(key[i])); // Vérifie que chaque caractère est alphanumérique
  }

  // Vérification 1.3 : Le dernier caractère doit être '\0'
  assert(key[key_len] == '\0');

  printf("Test 1 réussi : Clé de 16 caractères générée = %s\n", key);

  // Test 2 : Vérifier si la fonction peut générer des clés de tailles
  // différentes
  size_t key_sizes[] = {8, 32, 64};
  for (int i = 0; i < 3; i++) {
    size_t size = key_sizes[i];
    unsigned char key_dynamic[size + 1];
    gen_key(key_dynamic, size);

    // Vérification 2.1 : La clé doit avoir la longueur attendue
    assert(strlen((const char *)key_dynamic) == size);

    // Vérification 2.2 : Tous les caractères doivent être alphanumériques
    for (size_t j = 0; j < size; j++) {
      assert(isalnum(key_dynamic[j]));
    }

    // Vérification 2.3 : Le dernier caractère doit être '\0'
    assert(key_dynamic[size] == '\0');

    printf("Test 2.%d réussi : Clé de %zu caractères générée = %s\n", i + 1,
           size, key_dynamic);
  }

  // Test 3 : Vérifier que deux clés générées successivement sont différentes
  unsigned char key1[17];
  unsigned char key2[17];

  gen_key(key1, 16);
  gen_key(key2, 16);

  // Les deux clés doivent être différentes
  assert(strcmp((const char *)key1, (const char *)key2) != 0);

  printf(
      "Test 3 réussi : Deux clés générées successivement sont différentes.\n");

  printf("Tous les tests de gen_key sont réussis !\n");
  printf("-------Fin des tests de la fonctions gen_key------\n");
  printf("\n");
}

// Fonction de test pour xor_cipher
void test_xor_cipher() {
  printf("-------Début des tests de la fonction xor_cipher-------\n");

  // Test 1 : Vérifier que le chiffrement et déchiffrement fonctionnent
  // correctement
  unsigned char original_msg[] = "Les carottes sont cuites";
  size_t msg_len = strlen((const char *)original_msg);
  unsigned char msg[1024];
  strcpy((char *)msg, (const char *)original_msg);

  // Générer une clé pour le test
  size_t key_len = 16;
  unsigned char key[17];
  gen_key(key, key_len);

  // Chiffrer le message
  xor_cipher(msg, key, msg_len, key_len);

  // Déchiffrer le message (en utilisant la même clé)
  xor_cipher(msg, key, msg_len, key_len);

  // Vérifier que le message déchiffré est identique au message original
  assert(strcmp((const char *)msg, (const char *)original_msg) == 0);
  printf("Test 1 réussi : Chiffrement et déchiffrement donnent le message "
         "original.\n");

  // Test 2 : Vérifier que chiffrer deux fois avec la même clé donne le message
  // original
  strcpy((char *)msg, (const char *)original_msg);

  // Chiffrer deux fois
  xor_cipher(msg, key, msg_len, key_len);
  xor_cipher(msg, key, msg_len, key_len);

  // Vérifier que le message est revenu à l'original
  assert(strcmp((const char *)msg, (const char *)original_msg) == 0);
  printf("Test 2 réussi : Double chiffrement avec la même clé donne le message "
         "original.\n");

  // Test 3 : Vérifier que le chiffrement fonctionne correctement avec des
  // tailles de clé plus petites que le message
  unsigned char short_key[] = "abc"; // Clé de 3 caractères
  size_t short_key_len = strlen((const char *)short_key);

  // Copier le message original
  strcpy((char *)msg, (const char *)original_msg);

  // Chiffrer avec une petite clé
  xor_cipher(msg, short_key, msg_len, short_key_len);

  // Déchiffrer avec la même petite clé
  xor_cipher(msg, short_key, msg_len, short_key_len);

  // Vérifier que le message est revenu à l'original
  assert(strcmp((const char *)msg, (const char *)original_msg) == 0);
  printf("Test 3 réussi : Chiffrement avec une clé plus courte fonctionne "
         "correctement.\n");

  printf("Tous les tests de xor_cipher sont réussis !\n");
  printf("-------Fin des tests de la fonctions gen_key------\n");
  printf("\n");
}