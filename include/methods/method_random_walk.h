/**
 * @file method_random_walk.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief Méthode de marche aléatoire pour les problèmes "TSP"
 * @version 0.1
 * @date 2019-12-07
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef METHOD_RANDOM_WALK_H
#define METHOD_RANDOM_WALK_H

#include <time.h>
#include <tsplib/tsplib.h>

/**
 * @brief Construct a new random walk object
 *
 */
void random_walk(instance_t *, tour_t *);

#endif