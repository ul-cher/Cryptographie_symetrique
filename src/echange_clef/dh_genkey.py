import threading
import argparse
from queue import Queue


def puissance_mod_n(a: int, e: int, n: int) -> int:
    p = 1
    while e > 0:
        if e % 2 != 0:
            p = (p * a) % n
        a = (a * a) % n
        e = e // 2
    return p


def alice(q: Queue, a: int):
    print("Alice connaît sa clé privée: a = {}".format(a))
    A = puissance_mod_n(g, a, p)
    print("Alice calcule sa clé publique: A = g^a mod p = {}".format(A))
    q.put(A)
    print("Alice envoie A = {} sur le réseau.\n".format(A))


def bob(q: Queue, b: int):
    print("Bob connaît sa clé privée: b = {}".format(b))
    B = puissance_mod_n(g, b, p)
    print("Bob calcule sa clé publique: B = g^b mod p = {}".format(B))
    q.put(B)
    print("Bob envoie B = {} sur le réseau.\n".format(B))


# Parsing des arguments
parser = argparse.ArgumentParser(
    description="Générer une clé partagée Diffie-Hellman.")
parser.add_argument(
    "-i",
    "--input",
    required=True,
    help=
    "Fichier d'entrée contenant les paramètres publics et les clés privées.")
parser.add_argument("-o",
                    "--output",
                    required=True,
                    help="Fichier de sortie pour écrire la clé partagée.")
args = parser.parse_args()

# Lecture des paramètres du fichier param.txt
try:
    f = open(args.input, 'r')
    p = int(f.readline().strip())  # Nombre premier p (modulo)
    g = int(f.readline().strip())  # Générateur g
    a = int(f.readline().strip())  # Clé privée d'Alice
    b = int(f.readline().strip())  # Clé privée de Bob
    f.close()
except Exception as e:
    print("Erreur lors de la lecture du fichier d'entrée : {}".format(e))
    exit(1)

print("Paramètres publics : g = {}, p = {}\n".format(g, p))

# Création des queues pour l'échange de valeurs entre Alice et Bob
q_alice = Queue()
q_bob = Queue()

# Lancement des threads pour Alice et Bob
print("Démarrage de l'échange de clés:\n")
t_alice = threading.Thread(target=alice, args=(q_alice, a))
t_bob = threading.Thread(target=bob, args=(q_bob, b))
t_alice.start()
t_bob.start()

# Attente que les calculs d'Alice et Bob soient terminés
t_alice.join()
t_bob.join()

# Calcul de la clé partagée et écriture dans le fichier
A = q_alice.get()
B = q_bob.get()
print(
    "Eve observe les valeurs échangées sur le réseau: A = {}, B = {}\n".format(
        A, B))

K_A = puissance_mod_n(B, a, p)
print("Alice reçoit B = {} et calcule la clé partagée: K_A = B^a mod p = {}".
      format(B, K_A))

K_B = puissance_mod_n(A, b, p)
print("Bob reçoit A = {} et calcule la clé partagée: K_B = A^b mod p = {}\n".
      format(A, K_B))

if K_A == K_B:
    try:
        key_file = open(args.output, 'w')
        key_file.write("Clé partagée: {}\n".format(K_A))
        key_file.close()
        print("Clé partagée écrite dans {}: K = {}".format(args.output, K_A))
    except Exception as e:
        print("Erreur lors de l'écriture dans le fichier {}: {}".format(
            args.output, e))
else:
    print("Erreur : les clés partagées sont différentes.")
    exit(2)
