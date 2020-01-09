/**
 * @file method_nearest_neighbour.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief Méthodes heuristique des "plus proches voisins"
 * @version 0.1
 * @date 2019-12-23
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef METHOD_NEAREST_NEIGHBOUR_H
#define METHOD_NEAREST_NEIGHBOUR_H

#include <stdbool.h>
#include <tsplib/tsplib.h>

/**
 * @brief Applique la méthode de résolution suivant l'heuristique des plus
 * proches voisins.
 *
 * @param instance L'instance de problème TSP.
 * @param result La tournée solution.
 */
void nearest_neighbour(instance_t *instance, tour_t *result);

#endif