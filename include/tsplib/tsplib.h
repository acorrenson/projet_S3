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
} tour_t;

/**
 * @brief   Lecture d'un fichier au format TSP
 *
 * @return int  NIL en cas d'erreur(s), 0 sinon.
 */
int read_tsp_file(const char *, instance_t *);

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
 * @brief Mesure le nombre de noeuds d'un tour.
 *
 * @return double
 */
double tour__length(tour_t *);

/**
 * @brief Mesure la distance effective d'un tour.
 *
 * @return double
 */
double tour__effective_length(tour_t *);

/**
 * @brief Ajoute un noeud au tour.
 *
 * @return double
 */
double tour__add_node(tour_t *);

void instance__print_matrix(instance_t *);

#endif