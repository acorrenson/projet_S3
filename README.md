# projet_S3

Ce projet propose un solveur simple pour résoudre le fameux problème du **Traveling Salesman** ou **Voyageur de commerce**. Il s'appuie sur 4 méthodes bien connues :

1. **Brute-force** : recherche exhaustive, donne la solution optimale pour des toutes petites instances du problèmes (10 noeuds environ).
2. **Random-walk** : recherche aléatoire, donne une solution parfaitement aléatoire.
3. **Nearest-neighbour** : heuristique des "plus proches voisins". Donne une approximation de la solution en temps quadratique.
4. **Algorithmes génétiques** : utiliser le concept d'évolution pour approximer la solution en croisant plusieurs générations de candidats potentiels. On utilise en particulier une méthode appellé **croisement DPX**.

## Structure du code source

Le code source est organisé selon un schéma stricte:
+ `src/` : contient le code source en C
+ `src/tsplib/` : module d'interaction avec les fichiers au format TSP.
+ `src/methods/` : implémentation des différentes méthodes de résolution
+ `include/` : contient tous les en-têtes
+ `include/tsplib` : en-têtes pour le module tsplib
+ `include/methods` : en-têtes pour le méthodes de résolution
+ `tests/` : dossier contenant les modules de tests unitaires.

Notons que la séparation des sources et des en-têtes est un choix de design utilisé afin de faciliter l'usage de notre programme à la fois en tant que logicielle, mais également en tant que bibliothèque utilitaire. Le programmeur souhaitant re-exploiter tout ou partie de ces sources pourra aisément importer les en-têtes de son choix en passant l'option `-I include/` au compilateur C.

## Conventions de nommage

Pour fluidifier la lecture du code et de l'ensemble des fichiers sources, nous avons fixé les conventions de nommage suivantes :

+ **Nom de variable** : les noms de variables sont écris en ANGLAIS pour être compréhensibles par le plus grand nombre. Ils sont notés en convention *snake-case*.
+ **Nom de constante** : les constantes sont nommées en majuscules.
+ **Nom de fonction** : les noms de fonctions sont notés en ANGLAIS et suivent la convention *snake-case*. Ce choix est pris pour faciliter la compréhension du sens des fonctions. 
+ **Nom de procédures/fonctions sur les structures** : afin de reconnaître immédiatement les fonctions et procédures travaillant sur un type particulier de structure, ces fonctions voient leurs noms préfixés par le type de structure sur lequel elles opèrent suivi de deux symboles (`_`). Par exemple `instance__read_from_file` est une méthode opérant sur une structure de type `instance` et sons sens est transparent.
+ **Fichiers de tests** : les fichiers de tests sont nommés selon la syntaxe `nom_du_module_testé.test.c`. Il devient ainsi facile de les sélectionner tous en utilisant le filtre `*.test.c`.

## Processus de compilation

Le *pipeline* de compilation a été écris en CMake par soucis de clarté et de simplicité. Un makefile principale placé à la racine permet cependant de contrôler toutes les tâches liées au projet.

**Utilisation des commandes de compilation :**

```
// Compilation des sources
$ make build

// Lancement du programme principale avec paramètres de test (pour constater son bon fonctionnement)
$ make run

// lancement des tests unitaires
$ make test

// génération de la documentation Doxygen (dans les répertoires html/ et latex/)
$ make doc

// suppression des artefacts de compilation et réinitialisation des dossiers de build
$ make clean

// Lance le script d'affichage graphique des résultats dans data/results.csv
$ make graph
```

## Scripts utiles

Pour compléter le programme fournis, des scripts en python sont disponibles dans le répertoires `scripts/`. On trouve notamment le script `graph.py` permettant de visualiser le résultat produit par le programme `Tsp`. Son utilisation est comme suit :

```
python3 graph.py fichier1.csv
```

Où le fichier csv est conforme au format suivant :

```csv
Nom de l’instance ; (str)
Nb de villes ; (int)
Type ; TSP
Point ; Abs ; Ord
  (int) ; (int) ; (int)
  ...
Méthode ; longueur  ; Temps CPU (sec) ; Tour
(str)   ;   (float) ;   (float)       ; (int list)
```