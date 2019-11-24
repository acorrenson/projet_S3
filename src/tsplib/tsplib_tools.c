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
  tour->current = 0;
}

double instance__dist_euclidian(instance_t *instance, int a, int b) {
  assert(0 <= a && a < instance->dimension);
  assert(0 <= b && a < instance->dimension);

  int x1 = instance->tabCoord[a][0];
  int y1 = instance->tabCoord[a][1];
  int x2 = instance->tabCoord[b][0];
  int y2 = instance->tabCoord[b][1];

  double r = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
  // printf("(%d %d) (%d %d)\n", x1, x2, y1, y2);
  // printf("r [%d %d] = %f\n", a, b, r);
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double instance__dist_matrix(instance_t *instance, int a, int b) {
  assert(0 <= a && a < instance->dimension);
  assert(0 <= b && a < instance->dimension);

  // on utilise une demie matrice SUPERIEURE
  if (a < b)
    return instance->matDist[a][b];
  else
    return instance->matDist[b][a];
}

void instance__compute_distances(instance_t *instance) {
  instance->matDist = malloc(instance->dimension * sizeof(double *));
  for (int i = 0; i < instance->dimension; i++) {
    instance->matDist[i] = malloc(instance->dimension * sizeof(double));
  }

  for (int i = 0; i < instance->dimension; i++) {
    for (int j = i + 1; j < instance->dimension; j++) {
      instance->matDist[i][j] = instance__dist_euclidian(instance, i, j);
    }
  }
}

void tour__set_dimension(tour_t *t, int dim) {
  t->dimension = dim;
  t->tour = malloc(dim * sizeof(int));
}

void tour__add_node(tour_t *t, int node) {
  assert(0 <= node < t->dimension && t->current < t->dimension);
  t->tour[t->current] = node;
  t->current++;
}

double instance__compute_length(instance_t *instance) {
  int i = 0;
  instance->length = 0.0;
  while (i + 1 < instance->dimension) {
    instance->length += instance__dist_euclidian(instance, instance->tabTour[i],
                                                 instance->tabTour[i + 1]);
    i++;
  }
  instance->length += instance__dist_euclidian(
      instance, instance->tabTour[instance->dimension - 1],
      instance->tabTour[0]);
  return instance->length;
}

void instance__extract_tour(instance_t *instance, tour_t *tour) {
  tour__init(tour);
  tour__set_dimension(tour, instance->dimension);
  for (int i = 0; i < instance->dimension; i++) {
    tour__add_node(tour, instance->tabTour[i]);
  }
  strcpy(tour->name, instance->name);
  tour->length = instance->length;
}