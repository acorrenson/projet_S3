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
void instance__read_from_file(instance_t *, FILE *, bool);

/**
 * @brief Ecriture d'un tour dans un fichier.
 *
 */
void tour__write_to_file(tour_t *, FILE *);

void instance__write_coords_to_file(instance_t *instance, FILE *file);

/**
 * @brief Ecriture d'une instance dans un fichier
 *
 */
void instance__write_to_file(instance_t *, FILE *);

/**
 * @brief Affiche la matrice des distances d'une instance.
 *
 */
void instance__print_matrix(instance_t *);

/**
 * @brief Ecrire les coordonnées des sommets de la tournée en cours de calcul au
 * format csv.
 *
 * @param instance L'instance.
 * @param file Le fichier de sortie.
 */
void instance__save_to_csv(instance_t *instance, FILE *file);

/**
 * @brief Ecrire les coordonnées des sommets d'une tournée au
 * format csv.
 *
 * @param instance L'instance de départ.
 * @param tour La tournée.
 * @param file Le fichier de sortie.
 */
void tour__save_to_csv(instance_t *instance, tour_t *tour, FILE *file);

// ==================================================
// == INITIALISASIONS
// ==================================================

/**
 * @brief Initialisation d'une instance TSP.
 *
 */
void instance__init(instance_t *);

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
 * @brief Récupérer la distance qui sépare deux noeuds dans la matrice.
 *
 */
double instance__dist_matrix(instance_t *, int, int);

/**
 * @brief Calculer les distances entre tous les noeuds d'une instance TSP.
 *
 */
void instance__compute_distances(instance_t *);

/**
 * @brief Assure qu'un tour comprend un noeud.
 *
 * @return true
 * @return false
 */
bool tour__has_node(tour_t *, int);

/**
 * @brief Copie un tour à l'adresse souhaitée.
 *
 */
void tour__copy(tour_t *, tour_t);

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
 * @param instance L'instance de départ.
 * @param tour La tournée.
 * @return double
 */
double tour__compute_length(instance_t *instance, tour_t *tour, bool);

/**
 * @brief Extraire la tournée courante d'une instance et initialise une
 * structure tour avec.
 *
 */
void instance__extract_tour(instance_t *, tour_t *);

#endif