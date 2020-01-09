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
 * @brief     Recherche d'un fragment de chemin commun entre deux tournées
 * à partir de deux positions (recherche de droite à gauche dans la deuxième
 * tournée).
 *
 * @param t1  Pointeur sur la tournée 1.
 * @param t2  Pointeur sur la tournée 1.
 * @param i1  Indice de départ dans la tournée 1.
 * @param i2  Indice de départ dans la tournée 2.
 *
 * @pre t1->tour[i1] == t2->tour[i2] && t1->dimension == t2->dimension.
 *
 * @return int  Taille du fragment commun trouvé (au moins 1).
 */
int search_backward(tour_t *t1, tour_t *t2, int i1, int i2);

/**
 * @brief     Recherche d'un fragment de chemin commun entre deux tournées
 * à partir de deux positions (recherche de gauche à droite dans la deuxième
 * tournée).
 *
 * @param t1  Pointeur sur la tournée 1.
 * @param t2  Pointeur sur la tournée 2.
 * @param i1  Indice de départ dans la tournée 1.
 * @param i2  Indice de départ dans la tournée 2.
 *
 * @pre t1->tour[i1] == t2->tour[i2] && t1->dimension == t2->dimension.
 *
 * @return int  Taille du fragment commun trouvé (au moins 1).
 */
int search_forward(tour_t *t1, tour_t *t2, int i1, int i2);

/**
 * @brief Calcul des fragments de chemins partagés entre deux tournées.
 * L'allocation des ressources nécessaires au stockage des fragments est prise
 * en charge par la fonction.
 *
 * @param t1        La première tournée.
 * @param t2        La deuxième tournée.
 * @param fragments Les fragments partagés.
 * @param sizes     Les tailles des fragments.
 * @return int      Le nombre de fragments.
 */
int get_shared_fragments(tour_t *t1, tour_t *t2, int ***fragments, int **sizes);

/**
 * @brief Trouve la première occurrence de "false" dans un tableau booléen.
 *
 * @param marks   Le tableau.
 * @param size    La taille du tableau.
 *
 * @return int    L'indice de la première occurrence de false (ou NIL si
 * aucune).
 */
int find_fragment(int *marks, int size);

/**
 * @brief Trouve le fragment de chemin le plus proche d'un noeud parmis un
 * tableau de fragments de chemin. Ignore les fragments déclarés comme marqués.
 *
 * @param inst        L'instance.
 * @param node        Le noeud.
 * @param fragments   Le tableau de fragments.
 * @param size        Le nombre de fragments.
 * @param sizes       Les tailles des fragments.
 * @param marks       Tableau des marques sur les fragments.
 * @param reverse     Un booléen précisant si le chemin le plus proche doit être
 * inversé pour être connecté au noeud.
 *
 * @return int        L'indice du fragment le plus proche dans le tableau de
 * fragments.
 */
int nearest_fragment(instance_t *inst, int node, int **fragments, int size,
                     int *sizes, int *marks, bool *reverse);

/**
 * @brief Résout un problème TSP en appliquant un algorithme génétique.
 *
 */
void genetic(instance_t *, tour_t *, cli_opt_t *);

#endif
