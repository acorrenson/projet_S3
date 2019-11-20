#include <tsplib/tsplib.h>

void instance__init(instance_t *instance) {
  strcpy(instance->name, "NO_NAME");
  strcpy(instance->type, "NO_TYPE");
  instance->dimension = 0;
  instance->length = 0;
  instance->tabCoord = NULL;
  instance->matDist = NULL;
  instance->tabTour = NULL;
}

void tour__init(tour_t *tour) {
  strcpy(tour->name, "NO_NAME");
  tour->length = 0;
  tour->dimension = 0;
  tour->tour = NULL;
}
