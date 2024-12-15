def puissance_mod_n(a: int, e: int, n: int) -> int:
    p = 1
    while e > 0:
        if e % 2 != 0:
            p = (p * a) % n
        a = (a * a) % n
        e = e // 2
    return p


def test_puissance_mod_n():
    assert puissance_mod_n(5, 0, 7) == 1, "Erreur : Cas 1 - a^0 mod n"
    assert puissance_mod_n(5, 1, 7) == 5, "Erreur : Cas 2 - a^1 mod n"
    assert puissance_mod_n(5, 3, 13) == 8, "Erreur : Cas 3 - Valeur normale"
    assert puissance_mod_n(2, 100, 101) == 1, "Erreur : Cas 4 - Grand exposant"
    assert puissance_mod_n(
        123456789, 987654321, 1000000007
    ) == 652541198, "Erreur : Cas 5 - Base et exposant très grands"
    assert puissance_mod_n(5, 3, 1) == 0, "Erreur : Cas 6 - Modulo 1"
    assert puissance_mod_n(5, 2,
                           5) == 0, "Erreur : Cas 7 - Modulo égal à la base"
    print("Tous les tests ont réussi!")

# Exécuter les tests
test_puissance_mod_n()


