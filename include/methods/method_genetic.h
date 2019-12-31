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
#include <tsplib/tsplib.h>

#define MAX_POP 1000

/**
 * @brief Type représentant une population solution.
 *
 */
typedef tour_t population[MAX_POP];

/**
 * @brief Effectuer un croisement DPX entre deux individus.
 *
 */
void cross_dpx(instance_t *, tour_t *, tour_t *, tour_t *);

int explode(tour_t *t1, tour_t *t2, int ***fragments, int **sizes);

/**
 * @brief Résout un problème TSP en appliquant une
 *
 */
void genetic(instance_t *, tour_t *, cli_opt_t *);

#endif
