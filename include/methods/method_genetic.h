/**
 * @file method_genetic.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief
 * @version 0.1
 * @date 2019-12-28
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef METHODE_GENETIC_H
#define METHODE_GENETIC_H

#include <cli.h>
#include <methods/method_nearest_neighbour.h>
#include <methods/method_random_walk.h>
#include <tsplib/tsplib.h>

/**
 * @brief Effectuer un croisement DPX entre deux individus.
 *
 */
void cross_dpx(instance_t *, tour_t *, tour_t *, tour_t *);

/**
 * @brief Calcul des fragments de chemins partagés entre deux tournées.
 *
 * @param t1  La première tournée.
 * @param t2  La deuxième tournée.
 * @param fragments Les fragments partagés.
 * @param sizes     Les tailles des fragments.
 * @return int      Le nombre de fragments.
 */
int get_shared_fragments(tour_t *t1, tour_t *t2, int ***fragments, int **sizes);

/**
 * @brief Trouve la première occurence de "false" dans un tableau booléen.
 *
 * @param marks   Le tableau.
 * @param size    La taille du tableau.
 * @return int    L'indice de la première occurence de false (ou NIL si aucune).
 */
int find_fragment(int *marks, int size);

/**
 * @brief Find the nearest fragment of path to a node.
 *
 * @param inst        The instance.
 * @param node        The node.
 * @param fragments   The array of fragments.
 * @param size        The number of fragments.
 * @param sizes       The sizes of the fragments.
 * @param marks       Boolean array representing marked fragments.
 * @param reverse     A boolean set to True if the nearest fragment found should
 * be reversed to be connceted to the node.
 * @return int        The index of the nearest fragment in the array (or NIL if
 * none is found).
 */
int nearest_fragment(instance_t *inst, int node, int **fragments, int size,
                     int *sizes, int *marks, bool *reverse);

/**
 * @brief Résout un problème TSP en appliquant un algorithme génétique.
 *
 */
void genetic(instance_t *, tour_t *, cli_opt_t *);

#endif
