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
#ifndef METHOD_BRUTEFORCE_H
#define METHOD_BRUTEFORCE_H

#include <stdbool.h>
#include <tsplib/tsplib.h>

/**
 * @brief Calcul la prochaine permutation dans l'ordre lexicographique.
 *
 * @param permut  La permutation
 * @param dim     Nombre de valeur de la permutation
 * @return true   Si permut a été modifiée (n'est pas la dernière permutation)
 * @return false  Si permut n'a pas été modifiée (est la dernière permutation)
 */
bool next_permutation(int *permut, int dim);

void brute_force(instance_t *, tour_t *);

#endif