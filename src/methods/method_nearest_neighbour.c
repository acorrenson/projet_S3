#include <tsplib/tsplib.h>

/**
 * @brief Marque un noeud.
 *
 */
void _mark(instance_t *instance, int node) {
  assert(node < instance->dimension);
  instance->tabCoord[node][2] = 1;
}

/**
 * @brief Test si un noeud est marqué.
 *
 */
bool _marked(instance_t *instance, int node) {
  assert(node < instance->dimension);
  if (instance->tabCoord[node][2] == 0) {
    return false;
  } else {
    return true;
  }
}

/**
 * @brief Cherche le premier noeud non marqué.
 *
 */
int _find_non_marked(instance_t *instance) {
  for (int i = 0; i < instance->dimension; i++) {
    if (!_marked(instance, i))
      return i;
  }
  return NIL;
}

void nearest_neighbour(instance_t *instance, tour_t *result) {
  instance__compute_distances(instance);
  int dim = instance->dimension;
  int current_node = 0;
  _mark(instance, 0);
  int nearest_node = _find_non_marked(instance);
  double min_dist;
  double dist_to_next;
  int index = 1;

  instance->tabTour = malloc(dim * sizeof(int));
  instance->tabTour[0] = 0;

  while (nearest_node != NIL) {
    dist_to_next = instance__dist_matrix(instance, current_node, nearest_node);
    min_dist = dist_to_next;

    for (int i = 1; i < dim; i++) {
      if (!_marked(instance, i)) {
        dist_to_next = instance__dist_matrix(instance, current_node, i);
        if (dist_to_next < min_dist) {
          min_dist = dist_to_next;
          nearest_node = i;
        }
      }
    }

    instance->tabTour[index] = nearest_node;
    current_node = nearest_node;
    _mark(instance, current_node);
    nearest_node = _find_non_marked(instance);
    index++;
  }

  instance__extract_tour(instance, result);
}