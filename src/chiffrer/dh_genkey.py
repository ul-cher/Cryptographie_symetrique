import threading
from queue import Queue

def puissance_mod_n(a: int, e: int, n: int) -> int:
    return pow(a, e, n)


def test_puissance_mod_n():
    assert puissance_mod_n(5, 0, 7) == 1, "Erreur : Cas 1 - a^0 mod n"
    assert puissance_mod_n(5, 1, 7) == 5, "Erreur : Cas 2 - a^1 mod n"
    assert puissance_mod_n(5, 3, 13) == 8, "Erreur : Cas 3 - Valeur normale"
    assert puissance_mod_n(2, 100, 101) == 1, "Erreur : Cas 4 - Grand exposant"
    assert puissance_mod_n(123456789, 987654321,1000000007) == 652541198, "Erreur : Cas 5 - Base et exposant très grands"
    assert puissance_mod_n(5, 3, 1) == 0, "Erreur : Cas 6 - Modulo 1"
    assert puissance_mod_n(5, 2, 5) == 0, "Erreur : Cas 7 - Modulo égal à la base"
    print("Tous les tests ont réussi!")

# Exécuter les tests
#test_puissance_mod_n()

def alice(q: Queue, a: int, p: int, g: int):
    print("Alice connaît sa clé privée: a = {}".format(a))
    A = puissance_mod_n(g, a, p)
    print("Alice calcule sa clé publique: A = g^a mod p = {}".format(A))
    q.put(A)  #Alice envoie sa clé publique sur le réseau
    print("Alice envoie A = {} sur le réseau.\n".format(A))

def bob(q: Queue, b: int, p: int, g: int):
    print("Bob connaît sa clé privée: b = {}".format(b))
    B = puissance_mod_n(g, b, p)
    print("Bob calcule sa clé publique: B = g^b mod p = {}".format(B))
    q.put(B)  #Bob envoie sa clé publique sur le réseau
    print("Bob envoie B = {} sur le réseau.\n".format(B))


def calcul_cle(q_alice: Queue, q_bob: Queue, a: int, b: int, p: int):
    A = q_alice.get()
    B = q_bob.get()
    print("Eve observe les valeurs échangées sur le réseau: A = {}, B = {}\n".format(A, B))

    # Alice reçoit la clé publique de Bob et calcule la clé partagée
    K_A = puissance_mod_n(B, a, p)
    print("Alice reçoit B = {} et calcule la clé partagée: K_A = B^a mod p = {}".format(B, K_A))

    # Bob reçoit la clé publique d'Alice et calcule la clé partagée
    K_B = puissance_mod_n(A, b, p)
    print("Bob reçoit A = {} et calcule la clé partagée: K_B = A^b mod p = {}\n".format(A, K_B))

    # On vérifie que les clés sont identiques
    if K_A == K_B:
        print("Clé partagée réussie : K = {}".format(K_A))
    else:
        print("Erreur : les clés partagées sont différentes.")


# Lecture des paramètres du fichier param.txt
with open('param.txt', 'r') as f:
    p = int(f.readline().strip())  # Nombre premier p (modulo)
    g = int(f.readline().strip())  # Générateur g
    a = int(f.readline().strip())  # Exposant (clé privée d'Alice)
    b = int(f.readline().strip())  # Exposant (clé privée de Bob)

print("Paramètres publics : g = {}, p = {}\n".format(g, p))

# Création des queues pour l'échange de valeurs entre Alice et Bob
q_alice = Queue()
q_bob = Queue()

# Lancer les threads pour Alice et Bob
print("Démarrage de l'échange de clés\n")
t_alice = threading.Thread(target=alice, args=(q_alice, a, p, g))
t_bob = threading.Thread(target=bob, args=(q_bob, b, p, g))
t_alice.start()
t_bob.start()

# On attend que les calculs de Alice et Bob soient terminés
t_alice.join()
t_bob.join()

#On calcule la clé partagée
calcul_cle(q_alice, q_bob, a, b, p)

