/**
 * @file tsplib.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief
 * @version 0.1
 * @date 2019-11-20
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef TSPLIB_H
#define TSPLIB_H

#include <assert.h>
#include <math.h>
#include <pretty.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

/**
 * @brief Structure pour les instances des problèmes "TSP"
 *
 */
typedef struct instance_s {
  //! nom de l'instance
  char name[NAMESIZE];
  //! type de l'instance
  char type[NAMESIZE];
  //! nombre de sommets dans l'instance
  int dimension;
  //! longueur de la tournée (calcul)
  double length;
  //! tableau des coordonnées (x,y,marque=0 pas vu, n° ordre)
  int **tabCoord;
  //! demie matrice des distances euclidiennes (calcul)
  double **matDist;
  //! tableau des villes formant la tournée (dans l'ordre) (calcul)
  int *tabTour;
  //! prise en compte de la ville (0, 0) (true par défaut)
  bool node_zero;
} instance_t;

/**
 * @brief Structure pour les solutions aux problèmes "TSP"
 *
 */
typedef struct tour_s {
  //! nom de l'instance
  char name[NAMESIZE];
  //! nombre de sommets dans l'instance
  int dimension;
  //! longueur du tour
  double length;
  //! liste des noeuds de la tournée
  int *tour;
  //! noeud courant
  int current;
} tour_t;

// ==================================================
// == ENTREES - SORTIES
// ==================================================

/**
 * @brief   Lecture d'un fichier au format TSP. Peut interrompre l'éxecution du
 * programme en cas d'erreur de lecture.
 *
 */
void instance__read_from_file(instance_t *, FILE *);

/**
 * @brief Ecriture d'un tour dans un fichier (au format .tour).
 *
 */
void tour__write_as_tsp(tour_t *, FILE *);

/**
 * @brief   Affiche une tournée dans le style Python3 (lisible par eval).
 *
 * @param tour  Tournée.
 * @param out   Fichier de sortie.
 */
void tour__pprint(tour_t *tour, FILE *out);

/**
 * @brief Affiche la matrice des distances d'une instance.
 *
 */
void instance__print_matrix(instance_t *);

// ==================================================
// == INITIALISASIONS
// ==================================================

/**
 * @brief Initialisation d'une instance TSP.
 *
 * @param zero  Précise s'il l'ajout du noeud 0 est requis.
 */
void instance__init(instance_t *, bool zero);

/**
 * @brief Réinitialisation d'une instance TSP.
 * (Les marqueurs sont remis à 0).
 *
 */
void instance__reset(instance_t *);

/**
 * @brief Initialisation d'un tour TSP.
 *
 */
void tour__init(tour_t *);

/**
 * @brief Copie complète d'une tournée vers une autre.
 *
 * @pre La tournée déstination doit être initialisée et allouée (par exemple
 * avec la fonction @ref tour__set_dimension).
 */
void tour__copy(tour_t *, const tour_t *);

/**
 * @brief Initialise une tournée à partir d'un tableau d'entier.
 *
 * @param t     La tournée.
 * @param array Le tableau d'entiers.
 * @param dim   La dimension du tableau.
 */
void tour__from_array(tour_t *t, int array[], int dim);

/**
 * @brief Ajoute un noeud au tour.
 *
 * @return double
 */
void tour__add_node(tour_t *, int);

/**
 * @brief Initialise la dimension d'une tournée. Alloue la mémoire nécessaire.
 *
 * @param t   La tournée.
 * @param dim Dimension de la tournée.
 */
void tour__set_dimension(tour_t *t, int dim);

// ==================================================
// == FONCTIONS UTILES
// ==================================================

/**
 * @brief Calculer la distance euclidienne entre deux noeuds d'une instance
 * TSP.
 *
 */
double instance__dist_euclidian(instance_t *, int, int);

/**
 * @brief Récupérer la distance qui sépare deux noeuds dans la matrice des
 * distances (suppose que la méthode @ref instance__compute_distances ait déjà
 * été appellée au moins une fois avant).
 *
 */
double instance__dist_matrix(instance_t *, int, int);

/**
 * @brief Initialise la matrice des distances dans une instance TSP.
 *
 */
void instance__compute_distances(instance_t *);

/**
 * @brief Assure qu'un tour comprend un noeud donné.
 *
 * @return true
 * @return false
 */
bool tour__has_node(tour_t *, int);

/**
 * @brief Calculer la longueur de la tournée en cours de calcul dans une
 * instance.
 *
 * @return double
 */
double instance__compute_length(instance_t *, bool);

/**
 * @brief Calculer la longueur d'une tournée.
 *
 * @param instance L'instance de référence.
 * @param tour La tournée.
 * @return double
 */
double tour__compute_length(instance_t *instance, tour_t *tour, bool);

/**
 * @brief Extraire la tournée courante d'une instance et
 * initialise une structure @ref tour_t avec.
 *
 */
void instance__extract_tour(instance_t *, tour_t *);

/**
 * @brief Marque un noeud.
 *
 */
void instance__mark(instance_t *instance, int node);

/**
 * @brief Retirer la marque d'un noeud.
 *
 */
void instance__unmark(instance_t *instance, int node);

/**
 * @brief Test si un noeud est marqué.
 *
 */
bool instance__marked(instance_t *instance, int node);

/**
 * @brief Cherche le premier noeud non marqué.
 *
 */
int instance__find_non_marked(instance_t *instance);

/**
 * @brief Calcul le noeud d'indice "index" dans une instance.
 *
 * @param inst  L'instance.
 * @param index L'indice.
 * @return int  Le noeud d'indice "index".
 */
int instance__node_at(instance_t *inst, int index);

/**
 * @brief Calcul l'indice d'un noeud dans une instance.
 *
 * @param inst  L'instance.
 * @param node  Le noeud.
 * @return int  L'indice du noeud.
 */
int instance__index_of(instance_t *inst, int node);

/**
 * @brief   Trouve la position d'un noeud dans une tournée.
 *
 * @param t     La tournée.
 * @param node  Le noeud.
 * @return int  La position du noeud (ou NIL si le noeud est absent).
 */
int tour__index_of(tour_t *t, int node);

#endif