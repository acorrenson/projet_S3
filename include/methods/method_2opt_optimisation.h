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
 * @brief Clacul l'équation d'une droite passant par deux points.
 *
 * @param p1 Point 1
 * @param p2 Point 2
 * @return point_t  Equation de la droite (p1, p2)
 */
point_t line_of_points(point_t p1, point_t p2);

/**
 * @brief Clacul le point d'ntersection entre deux droites.
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
 * @brief Clacul le centre d'un segment.
 *
 * @param p1  Extrémité 1 du segment.
 * @param p2  Extrémité 2 du segment.
 * @return    Centre.
 */

point_t center_of_segment(point_t p1, point_t p2);

/**
 * @brief Test si deux segements se croisent.
 *
 * @param p1  Extrémité 1 du segment 1.
 * @param p2  Extrémité 2 du segment 1.
 * @param p3  Extrémité 1 du segment 2.
 * @param p4  Extrémité 2 du segment 2.
 */
bool cross(point_t p1, point_t p2, point_t p3, point_t p4);

bool optimize_2opt(instance_t *instance);

#endif
