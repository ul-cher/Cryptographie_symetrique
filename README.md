# **SymCrypt - Cryptographie Symétrique**

## **Description**

**SymCrypt** est un programme permettant de chiffrer et déchiffrer des fichiers en utilisant plusieurs méthodes de cryptographie symétrique. Le logiciel inclut également un module pour l’échange sécurisé de clés (Diffie-Hellman) et un module pour le crackage de chiffrement.

## **Fonctionnalités**

### **Chiffrement et Déchiffrement**
- **XOR** : Chiffrement simple bit à bit avec une clé répétée.
- **MASK (One-Time Pad)** : Chiffrement par masque jetable.
- **CBC (Cipher Block Chaining)** : Chiffrement par blocs avec un vecteur d’initialisation.

### **Gestion des Clés**
- **Génération de clés aléatoires** : Permet de générer des clés de chiffrement.
- **Stockage des clés générées** : Sauvegarde des clés générées pour un usage ultérieur.
- **Suppression des clés utilisées** : Nettoyage sécurisé des clés après usage.

### **Échange Sécurisé de Clé (Diffie-Hellman)**
- **Génération des paramètres du groupe** : Nombre premier et générateur pour Diffie-Hellman.
- **Calcul des clés publiques et privées** : Processus de création des clés publiques et privées pour Alice et Bob.
- **Génération d’une clé partagée** : Création d'une clé secrète partagée entre Alice et Bob via un canal non sécurisé.

### **Crackage de Chiffrement**
- **Réduction des caractères possibles d’une clé** : Réduction de l’espace de recherche pour faciliter le crackage.
- **Analyse statistique des fréquences de lettres** : Utilisation de la statistique pour identifier des motifs dans le texte chiffré.
- **Attaque basée sur un dictionnaire** : Tentatives de décryptage en utilisant un dictionnaire préexistant pour tester les clés possibles.

## **Compilation**

Pour compiler le programme, exécutez la commande suivante dans le terminal :

```bash
make
