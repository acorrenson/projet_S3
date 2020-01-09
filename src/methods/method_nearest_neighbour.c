#include <methods/method_nearest_neighbour.h>

void nearest_neighbour(instance_t *instance, tour_t *result) {
  instance__compute_distances(instance);
  int dim = instance->dimension;
  int current_node = instance__node_at(instance, 0);
  instance__mark(instance, current_node);
  int nearest_node = instance__find_non_marked(instance);
  double min_dist;
  double dist_to_next;
  int index = 1;

  instance->tabTour = malloc(dim * sizeof(int));
  instance->tabTour[0] = current_node;

  while (nearest_node != NIL) {
    instance__mark(instance, nearest_node);
    dist_to_next = instance__dist_matrix(instance, current_node, nearest_node);
    min_dist = dist_to_next;

    for (int i = 1; i < dim; i++) {
      int node_i = instance__node_at(instance, i);
      if (!instance__marked(instance, node_i)) {
        dist_to_next = instance__dist_matrix(instance, current_node, node_i);
        if (dist_to_next < min_dist) {
          instance__unmark(instance, nearest_node);
          min_dist = dist_to_next;
          nearest_node = node_i;
          instance__mark(instance, nearest_node);
        }
      }
    }
    instance->tabTour[index] = nearest_node;
    current_node = nearest_node;
    assert(instance__marked(instance, nearest_node));
    nearest_node = instance__find_non_marked(instance);
    index++;
  }

  instance__compute_length(instance, true);
  instance__extract_tour(instance, result);
}