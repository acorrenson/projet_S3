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
 * @brief   Utilise la méthode heuristique "Marche Aléatoire" pour résoudre un
 * problème TSP.
 *
 * @param instance  Instance de problème TSP.
 * @param tour      Tournée solution au problème TSP.
 */
void random_walk(instance_t *instance, tour_t *tour);

#endif