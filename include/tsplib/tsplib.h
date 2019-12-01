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
  char name[TAILLENOM]; // nom de l'instance
  char type[TAILLENOM]; // type de l'instance
  int dimension;        // nombre de sommets dans l'instance
  double length;        // longueur de la tournée (calcul)
  int **tabCoord;   // tableau des coordonnées (x,y,marque=0 pas vu, n° ordre)
  double **matDist; // demie matrice des distances euclidiennes (calcul)
  int *tabTour; // tableau des villes formant la tournée (dans l'ordre) (calcul)
} instance_t;

/**
 * @brief Structure pour les solutions aux problèmes "TSP"
 *
 */
typedef struct tour_s {
  char name[TAILLENOM]; // nom de l'instance
  int dimension;        // nombre de sommets dans l'instance
  double length;        // longueur du tour (calculée ou lue)
  int *tour; // liste des noeuds de la tournée lus dans le fichier tour
  int current;
} tour_t;

// ==================================================
// == ENTREES - SORTIES
// ==================================================

/**
 * @brief   Lecture d'un fichier au format TSP
 *
 */
void instance__read_from_file(instance_t *, const char *);

/**
 * @brief   Ecriture d'un tour dans un fichier.
 *
 */
void tour__write_to_file(tour_t *, FILE *);

void instance__write_graph_to_file(instance_t *instance, FILE *file, int mag);

void tour__write_graph_to_file(tour_t *instance, FILE *file);

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

// ==================================================
// == INITIALISASIONS
// ==================================================

/**
 * @brief Initialisation d'une instance TSP.
 *
 */
void instance__init(instance_t *);

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

double instance__compute_length(instance_t *);

/**
 * @brief Extraire la tournée courante d'une instance et initialise une
 * structure tour avec.
 *
 */
void instance__extract_tour(instance_t *, tour_t *);

/**
 * @brief Copie de la tournée courante d'une instance dans une structure tour
 * déjà initialisée pour l'instance (avec @ref instance__extract_tour
 * typiquement).
 *
 */
void instance__set_tour(instance_t *, tour_t *);

#endif