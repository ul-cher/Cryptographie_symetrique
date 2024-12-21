# SymCrypt
SymCrypt est un programme de chiffrement/déchiffrement qui implémente plusieurs méthodes, notamment **XOR**, **CBC**, et **MASK**.  
Il est conçu pour traiter des fichiers texte et offre une sécurité de base pour la gestion des données sensibles.  

---

## Fonctionnalités principales
1. XOR : Un chiffrement simple qui utilise une clé répétée pour chiffrer/déchiffrer les données.  
2. CBC (Cipher Block Chaining) : Une méthode de chiffrement avancée basée sur des blocs et utilisant un vecteur d'initialisation (IV).  
3. MASK : Une méthode complémentaire pour manipuler des données avec une couche de masquage supplémentaire.  

---

## Structure du Projet
Le programme est organisé en plusieurs fichiers :  
- sym_crypt.c : Point d'entrée du programme. Il gère les arguments, les entrées/sorties et appelle les fonctions appropriées.  
- cbc.c : Implémente le chiffrement et le déchiffrement en mode CBC.  
- xor.c : Implémente le chiffrement/déchiffrement XOR.  
- headers : Fichiers d'en-tête contenant les définitions et prototypes nécessaires.  

---

## Instructions d'Utilisation

### Commande de base
```bash
./symcrypt -i <input_file> -o <output_file> -k <cle>/-f <key_file> -m <methode> -v <iv_file> -l <log_file>

### Exemple
Chiffrement XOR :
./symcrypt -i input.txt -o output.txt -k my_secret_key -m xor

Chiffrement CBC :
./symcrypt -i input.txt -o output.txt -f key.bin -m cbc-crypt -v iv.bin

Déchiffrement CBC :
./symcrypt -i output.txt -o decrypted.txt -f key.bin -m cbc-decrypt -v iv.bin

Méthode MASK :
./symcrypt -i input.txt -o masked.txt -k my_secret_key -m mask

## Afficher l'aide :
./symcrypt -h
