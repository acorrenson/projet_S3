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
  int random_index = (rand() % (instance->dimension - 1)) + 1;
  while (instance->tabCoord[random_index][2] == 1) {
    random_index = (rand() % (instance->dimension - 1)) + 1;
  }

  return random_index;
}

void random_walk(instance_t *instance, tour_t *tour) {
  srand(time(NULL));
  int random_node;
  tour__init(tour);
  tour__set_dimension(tour, instance->dimension);
  tour__add_node(tour, 0);
  instance->tabCoord[0][2] = 1;

  for (int i = 1; i < instance->dimension; i++) {
    random_node = peek_one_randomly(instance);
    assert(random_node >= 0);
    assert(random_node < instance->dimension);
    instance->tabCoord[random_node][2] = 1;
    tour__add_node(tour, random_node);
  }
  tour__compute_length(instance, tour, true);
}