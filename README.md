# 🛠️ Optimisation de get\_next\_line : gestion améliorée du buffer

## Table des matières

1. [Description](#description)
2. [Fonctionnalités clés](#fonctionnalites-clés)
3. [Structure interne `t_dynbuf`](#structure-interne-tdynbuf)
4. [Rôles des fonctions](#rôles-des-fonctions)
5. [Bénéfices observés](#bénéfices-observés)
6. [Tests et résultats](#tests-et-résultats)
7. [Exemple d'utilisation](#exemple-dutilisation)

---

## Description

Cette version de **get\_next\_line** se concentre sur une **gestion mémoire optimisée** et une **performance accrue**, même pour des fichiers très volumineux ou contenant des lignes extrêmement longues (sans terminaison par `\n`). La concaténation coûteuse de chaînes est remplacée par un tampon dynamique, garantissant une lecture ligne par ligne efficace et sans fuite mémoire.

---

## Fonctionnalités clés

* 📦 **Buffer extensible dynamique** : remplace `ft_strjoin_free` par la structure `t_dynbuf`, qui gère un tampon évolutif.
* 🔄 **Lecture conditionnelle** : on arrête la lecture dès la détection d'un `\n`, sans appels superflus à `read()`.
* 🧠 **Réduction des copies** : travail direct dans le tampon, minimisant les opérations `memcpy` répétées.
* 📈 **Croissance géométrique** : la capacité du tampon double automatiquement pour limiter le nombre de `malloc`.
* 💡 **Robustesse sur lignes longues** : supporte des fichiers sans `\n` jusqu'à 1 000 000 de caractères.

---

## Structure interne `t_dynbuf`

La structure `t_dynbuf` centralise l'état du buffer dynamique :

```c
typedef struct s_dynbuf {
    char    *data;     // Pointeur vers le contenu accumulé
    size_t   len;      // Nombre d'octets valides dans data
    size_t   capacity; // Capacité allouée de data en octets
} t_dynbuf;
```

* **`data`** : zone mémoire stockant les octets lus.
* **`len`** : longueur actuelle des données.
* **`capacity`** : taille maximale allouée, réajustée par `extend_buf`.

---

## Rôles des fonctions

1. **`get_next_line(int fd)`**

   * Point d’entrée : renvoie la ligne suivante (jusqu'au `\n`) ou `NULL` en fin de fichier/erreur.
   * Valide `fd` et `BUFFER_SIZE`.
   * Appelle `read_file` pour accumuler les données.
   * Extrait la ligne dans un buffer alloué et met à jour le "reste" via `next_line`.

2. **`read_file(int fd, char *stock)`**

   * Initialise `buff` et `t_dynbuf buf`.
   * Réinjecte le `stock` existant dans `buf` pour ne rien perdre.
   * Lit une première fois et délègue à `read_loop` pour compléter jusqu’au `\n`.

3. **`read_loop(int fd, char *buff, ssize_t br, t_dynbuf buf)`**

   * Boucle tant que `br > 0` :

     * Vérifie et étend `buf` via `extend_buf` si nécessaire.
     * Copie `buff` dans `buf.data`, met à jour `len` et termine par `\0`.
     * Arrêt dès détection de `\n` dans `buff`.
   * Libère `buff` et renvoie le contenu accumulé (`buf.data`) ou `NULL` en cas d’erreur.

4. **`extend_buf(t_dynbuf *buf, ssize_t br)`**

   * Calcule une nouvelle capacité (double ou `br+1`).
   * Alloue `new_data`, copie l’ancien contenu, libère l’ancien tampon.
   * Met à jour `buf->data` et `buf->capacity`.
   * Renvoie `1` si succès, `0` sinon.

5. **`next_line(char *stock)`**

   * Localise le premier `\n` dans `stock`.
   * Si trouvé, duplique la partie après le `\n` comme nouveau `stock`.
   * Sinon, libère `stock` et renvoie `NULL`.

---

## Bénéfices observés

* ⚡ **Performance** : temps d'exécution réduit de moitié sur lignes très longues.
* ✅ **Stabilité** : plus de timeouts sur fichiers > 100 000 lignes.
* 💾 **Mémoire** : réallocations minimisées et pas de fuite (Valgrind ✅).

---

## Tests et résultats

| Test                            | Résultat attendu                 |
| ------------------------------- | -------------------------------- |
| Fichier binaire (ex. vidéo)     | Retour immédiat de `NULL`        |
| Fichier à ligne unique          | Retour exact (avec ou sans `\n`) |
| Fichier 1 000 000+ caractères   | Pas de timeout ni de segfault    |
| Fuite mémoire (Valgrind)        | 0 fuite détectée                 |
| `TAILLE_TAMPON` de 1 à 100 000+ | Fonctionnement stable            |

---

## Exemple d'utilisation

```c
int fd = open("mon_fichier.txt", O_RDONLY);
char *line;
while ((line = get_next_line(fd))) {
    printf("%s", line);
    free(line);
}
close(fd);
```

---

## Exemple d'exécution pas à pas

Prenons un fichier dont le contenu est :

```text
Hello

World

```

et `BUFFER_SIZE = 8`. Nous appelons trois fois `get_next_line(fd)` pour extraire successivement chaque ligne.

### État initial

* **`stock = NULL`**
* Fichier positionné à l’octet 0 : `H e l l o 
   W o r l d 
  `

### Appel #1 à `get_next_line(fd)`

1. **Initialisation**

   * `stock = NULL`
   * Vérifications : `fd` valide, `BUFFER_SIZE > 0`, `read(fd, NULL,0)` ok.
2. **Lecture via `read_file`**

   * `buff = malloc(8)`
   * `buf.data = NULL, buf.len = 0, buf.capacity = 0`
   * `br = read(fd, buff, 8)` → lit `"Hello
     Wo"` (8 octets)
   * Appel de `read_loop(fd, buff, 8, buf)`
3. **Dans `read_loop`**

   * **Extend** : capacité 0 → new\_cap = 8+1 = 9
   * **Copie** : `buf.data = "Hello
     Wo"`, `buf.len = 8`, `'�'` ajouté
   * \*\*Détection
     \*\* : trouvé à l’indice 5 → sortie
   * **Retour** : `buf.data = "Hello
     Wo"`
4. **Extraction de la ligne**

   * `len = 6` ("Hello
     ")
   * `line = malloc(7); strlcpy(line, stock, 7)` → `"Hello
     "`
5. **Mise à jour `stock`**

   * `next_line("Hello
     Wo")` → `"Wo"`

> **Résultat appel #1** : renvoie `"Hello
> "`, nouveau `stock = "Wo"`

### Appel #2 à `get_next_line(fd)`

1. **Initialisation**

   * `stock = "Wo"`
2. **Lecture via `read_file`**

   * `buff = malloc(8)`
   * Réinjection : `buf.data = "Wo"`, `buf.len = 2`, `buf.capacity = 3`
   * `br = read(fd, buff, 8)` → lit `"rld
     "` (4 octets)
   * Appel de `read_loop(fd, buff, 4, buf)`
3. **Dans `read_loop`**

   * **Extend** : capacity 3 < 2+4+1=7 → new\_cap = 6 → <7 → new\_cap=12
   * **Copie** : `buf.data = "World
     "`, `buf.len = 6`
   * \*\*Détection
     \*\* : trouvé à l’indice 3 → sortie
   * **Retour** : `buf.data = "World
     "`
4. **Extraction de la ligne**

   * `len = 6`; `line = "World
     "`
5. **Mise à jour `stock`**

   * `next_line("World
     ")` → `NULL`

> **Résultat appel #2** : renvoie `"World
> "`, nouveau `stock = NULL`

### Appel #3 à `get_next_line(fd)`

* `stock = NULL`
* `read_file(fd, NULL)` lit `br = 0` (EOF)
* `read_loop` renvoie `buf.data = NULL`
* `get_next_line` renvoie `NULL` (fin de fichier)
