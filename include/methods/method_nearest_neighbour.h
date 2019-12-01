/**
 * @file method_bruteforce.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief Method de recherche exhaustive pour les problèmes "TSP".
 * @version 0.1
 * @date 2019-11-23
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef METHOD_NEAREST_NEIGHBOUR_H
#define METHOD_NEAREST_NEIGHBOUR_H

#include <stdbool.h>
#include <tsplib/tsplib.h>

void mark(instance_t *, int);
bool marked(instance_t *, int);
void nearest_neighbour(instance_t *instance, tour_t *result);

#endif