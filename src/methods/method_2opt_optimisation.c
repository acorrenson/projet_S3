#include <methods/method_2opt_optimisation.h>

point_t line_of_points(point_t p1, point_t p2) {
  assert(p1.x != p2.x || p1.y != p2.y);
  double m = (p1.y - p2.y) / (p1.x - p2.x);
  double p = p1.y - (m * p1.x);
  point_t line = {m, p};
  return line;
}

point_t intersection(point_t d1, point_t d2) {
  assert(d1.x != d2.x);
  double a = (d2.y - d1.y) / (d1.x - d2.x);
  double b = d1.x * a + d1.y;
  point_t inter = {a, b};
  return inter;
}

bool interesect(point_t d1, point_t d2) { return d1.x != d1.y; }

double dist(point_t p1, point_t p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

point_t center_of_segment(point_t p1, point_t p2) {
  double a, b;
  a = (p2.x + p1.x) / 2;
  b = (p2.y + p1.y) / 2;

  point_t center = {a, b};
  return center;
}

bool cross(point_t p1, point_t p2, point_t p3, point_t p4) {
  point_t d1 = line_of_points(p1, p2);
  point_t d2 = line_of_points(p3, p4);
  point_t center1 = center_of_segment(p1, p2);
  point_t center2 = center_of_segment(p3, p4);
  point_t inter = intersection(d1, d2);
  return dist(inter, center1) < dist(p1, p2) / 2 &&
         dist(inter, center2) < dist(p3, p4) / 2;
}

bool optimize_2opt(instance_t *instance, tour_t *tour) {
  int dim = instance->dimension;
  for (int i = 0; i < dim - 1; i++) {
    for (int j = 0; j < dim; j++) {
      if (i != j && i + 1 != (j + 1) % dim && i + 1 != j &&
          (j + 1) % dim != i) {
        int n1 = tour->tour[i];
        int n2 = tour->tour[i + 1];
        int n3 = tour->tour[j];
        int n4 = tour->tour[(j + 1) % dim];
        point_t p1 = {instance->tabCoord[n1][0], instance->tabCoord[n1][1]};
        point_t p2 = {instance->tabCoord[n2][0], instance->tabCoord[n2][1]};
        point_t p3 = {instance->tabCoord[n3][0], instance->tabCoord[n3][1]};
        point_t p4 = {instance->tabCoord[n4][0], instance->tabCoord[n4][1]};
        if (cross(p1, p2, p3, p4)) {
          printf("POSSIBLE OPT (%d %d)x(%d %d)!\n", n1, n2, n3, n4);
          // decroiser
          // -> inverser le chemin entre Noeud(i+1) et Noeud(j) (inclus)
          reverse(tour->tour, (i + 1) % dim, j % dim);
          tour__compute_length(instance, tour);
          return true;
        }
      }
    }
  }
  return false;
}