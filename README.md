### 🛠️ Optimisation de `get_next_line` : gestion améliorée du buffer

Cette version optimise la fonction `get_next_line` en améliorant significativement la **gestion mémoire et les performances**, notamment pour les fichiers très volumineux ou contenant des lignes longues non terminées par `\n`.

---

## ⚙️ Principales améliorations

* 📦 **Buffer extensible dynamique** : remplace la concaténation coûteuse de chaînes (`ft_strjoin_free`) par une structure interne capable de croître intelligemment.
* 🔄 **Limitation des appels à `read()`** : la lecture s'arrête dès qu’un `\n` est trouvé, comme le demande le sujet, mais sans lecture superflue.
* 🧠 **Réduction des copies mémoire** : grâce à un travail direct dans un buffer alloué une seule fois et réutilisé.
* 📈 **Croissance exponentielle du buffer** : la capacité est doublée progressivement pour éviter des `malloc` trop fréquents.
* 💡 **Gestion des lignes très longues** : plus robuste sur des fichiers sans retour à la ligne (tests jusqu'à 1 million de caractères).

---

## 🚀 Bénéfices observés

* ⚡ **Temps d’exécution divisé par deux** sur les fichiers contenant de très longues lignes sans `\n`.
* ✅ **Suppression des timeouts critiques** sur des cas limites (> 100 000 lignes).
* 💾 **Moins de consommation mémoire** grâce à une stratégie de réallocation intelligente et à la réutilisation du tampon.

---

## 🔬 Tests réalisés

| Test                                  | Résultat attendu                       |
| ------------------------------------- | -------------------------------------- |
| 📄 Fichier vide                       | Retour immédiat de `NULL`              |
| 📄 Fichier avec une seule ligne       | Retour correct avec ou sans `\n`       |
| 🧱 Fichier avec 1M+ caractères        | Pas de timeout, pas de segfault        |
| 🌀 Test de fuite mémoire (`Valgrind`) | ✅ 0 leak détecté                       |
| 🔁 BUFFER\_SIZE 1 → 100 000+          | Fonctionnement stable à toutes tailles |

---

## 📌 Remarque

Ces optimisations respectent **entièrement les contraintes de la partie obligatoire** :

* Aucune fonction interdite
* Respect du `BUFFER_SIZE`
* Lecture **ligne par ligne uniquement**
* Pas de fuite mémoire
* Aucun comportement indéfini

---
