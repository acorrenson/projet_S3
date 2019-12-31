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
  char name[TAILLENOM];
  //! type de l'instance
  char type[TAILLENOM];
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
  char name[TAILLENOM];
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
 * @brief   Lecture d'un fichier au format TSP
 *
 */
void instance__read_from_file(instance_t *, FILE *);

/**
 * @brief Ecriture d'un tour dans un fichier (au format .tour).
 *
 * @param zero Précise si la numérotation des noeuds commence à zero ou le cas
 * contraire à 1.
 */
void tour__write_as_tsp(tour_t *, FILE *);

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
 */
void instance__init(instance_t *, bool);

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
 * @brief Ajoute un noeud au tour.
 *
 * @return double
 */
void tour__add_node(tour_t *, int);

/**
 * @brief Initialise la dimension d'une tournée.
 *
 */
void tour__set_dimension(tour_t *, int);

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
 * @brief Marque un neoud.
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
 * @brief Extraire la list des arrêtes d'une tournée (les couples arrêtes sont
 * donnés dans l'ordre croissant).
 *
 * @param t     La tournée.
 * @param edges Les arrêtes.
 */
void tour__get_edges(tour_t *t, int ***edges);

#endif