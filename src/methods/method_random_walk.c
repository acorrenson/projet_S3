#include <methods/method_random_walk.h>

bool one_is_not_marked(instance_t *instance) {
  for (int i = 0; i < instance->dimension; i++) {
    if (instance->tabCoord[i][2] == 0)
      return true;
  }
  return false;
}

int peek_one_randomly(instance_t *instance) {
  assert(one_is_not_marked(instance));
  int random_index = rand() % instance->dimension;
  while (
      instance->tabCoord[random_index][2] == 1 ||
      instance__marked(instance, instance__node_at(instance, random_index))) {
    random_index = rand() % instance->dimension;
  }

  return random_index;
}

void random_walk(instance_t *instance, tour_t *tour) {
  int random_node_i;

  // initialisation de la tournée
  tour__init(tour);
  tour__set_dimension(tour, instance->dimension);
  tour__add_node(tour, instance__node_at(instance, 0));

  // réinitialisation de l'instance
  instance__reset(instance);
  instance__mark(instance, instance__node_at(instance, 0));

  for (int i = 1; i < instance->dimension; i++) {
    random_node_i = peek_one_randomly(instance);
    instance__mark(instance, instance__node_at(instance, random_node_i));
    tour__add_node(tour, instance__node_at(instance, random_node_i));
  }
  tour__compute_length(instance, tour, true);
}