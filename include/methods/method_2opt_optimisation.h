/**
 * @file method_2opt_optimisation.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief Fonctions d'optimisation des tournées par décroisement des chemins.
 * @version 0.1
 * @date 2019-12-23
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef METHOD_2OPT_H
#define METHOD_2OPT_H

#include <math.h>
#include <tsplib/tsplib.h>

/**
 * @brief Type pour représenter un point du plan.
 *
 */
typedef struct {
  double x;
  double y;
} point_t;

/**
 * @brief Calcul l'équation d'une droite passant par deux points.
 *
 * @param p1 Point 1
 * @param p2 Point 2
 * @return point_t  Equation de la droite (p1, p2)
 */
point_t line_of_points(point_t p1, point_t p2);

/**
 * @brief Calcul le point d'intersection entre deux droites.
 *
 * @param d1  Droite 1
 * @param d2  Droite 2
 * @return point_t  Intersection
 */
point_t intersection(point_t d1, point_t d2);

/**
 * @brief Test si deux droites se croisent.
 *
 */
bool interesect(point_t d1, point_t d2);

/**
 * @brief Distance euclidienne entre deux points
 *
 */
double dist(point_t p1, point_t p2);

/**
 * @brief Calcul le centre d'un segment.
 *
 * @param p1  Extrémité 1 du segment.
 * @param p2  Extrémité 2 du segment.
 * @return    Centre.
 */
point_t center_of_segment(point_t p1, point_t p2);

/**
 * @brief Test si deux segments se croisent.
 *
 * @param p1  Extrémité 1 du segment 1.
 * @param p2  Extrémité 2 du segment 1.
 * @param p3  Extrémité 1 du segment 2.
 * @param p4  Extrémité 2 du segment 2.
 */
bool cross(point_t p1, point_t p2, point_t p3, point_t p4);

/**
 * @brief Effectue l'optimisation d'une tournée.
 *
 * @param instance Instance d'origine.
 * @param tour Tournée à optimiser.
 * @return true   Si l'optimisation a pu être appliquée
 * @return false  Si l'optimisation n'a pu être appliquée
 */
bool optimize_2opt(instance_t *instance, tour_t *tour);

#endif
