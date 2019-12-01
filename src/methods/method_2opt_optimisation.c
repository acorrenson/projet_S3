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
point_t line_of_points(point_t p1, point_t p2) {
  assert(p1.x != p2.x || p1.y != p2.y);
  double m = (p1.y - p2.y) / (p1.x - p2.x);
  double p = p1.y - (m * p1.x);
  point_t line = {m, p};
  return line;
}

/**
 * @brief Clacul le point d'ntersection entre deux droites.
 *
 * @param d1  Droite 1
 * @param d2  Droite 2
 * @return point_t  Intersection
 */
point_t intersection(point_t d1, point_t d2) {
  assert(d1.x != d2.x);
  double a = (d2.y - d1.y) / (d1.x - d2.x);
  double b = d1.x * a + d1.y;
  point_t inter = {a, b};
  return inter;
}

/**
 * @brief Test si deux droites se croisent.
 *
 */
bool interesect(point_t d1, point_t d2) { return d1.x != d1.y; }

/**
 * @brief Distance euclidienne entre deux points
 *
 */
double dist(point_t p1, point_t p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/**
 * @brief Clacul le centre d'un segment.
 *
 * @param p1  Extrémité 1 du segment.
 * @param p2  Extrémité 2 du segment.
 * @return    Centre.
 */
point_t center_of_segment(point_t p1, point_t p2) {
  double a;
  if (p1.x < p2.x)
    a = (p2.x - p1.x) / 2;
  else
    a = (p1.x - p2.x) / 2;

  double b;
  if (p1.y < p2.y)
    b = (p2.y - p1.y) / 2;
  else
    b = (p1.y - p2.y) / 2;

  point_t center = {a, b};
  return center;
}

/**
 * @brief Test si deux segements se croisent.
 *
 * @param p1  Extrémité 1 du segment 1.
 * @param p2  Extrémité 2 du segment 1.
 * @param p3  Extrémité 1 du segment 2.
 * @param p4  Extrémité 2 du segment 2.
 */
bool cross(point_t p1, point_t p2, point_t p3, point_t p4) {
  point_t d1 = line_of_points(p1, p2);
  point_t d2 = line_of_points(p3, p4);
  point_t center = center_of_segment(p1, p2);
  return dist(intersection(d1, d2), center) <= dist(p1, p2) / 2;
}

// int main() {
//   point_t p1 = {0, 0};
//   point_t p2 = {1, 1};
//   point_t p3 = {0, 0.5};
//   point_t p4 = {1, 2};

//   printf("intersect ? %d\n",
//          interesect(line_of_points(p1, p2), line_of_points(p3, p4)));

//   printf("cross ? %d\n", cross(p1, p2, p3, p4));
// }
