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

void instance__reset(instance_t *instance) {
  for (int i = 0; i < instance->dimension; i++) {
    instance->tabCoord[i][2] = 0;
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

double tour__compute_length(instance_t *instance, tour_t *tour) {
  assert(tour->dimension == instance->dimension);
  int i = 0;
  tour->length = 0.0;

  for (int i = 0; i < instance->dimension; i++) {
    int n1 = tour->tour[i];
    int n2 = tour->tour[(i + 1) % (instance->dimension)];
    double dist = instance__dist_euclidian(instance, n1, n2);
    tour->length += dist;
  }
  return tour->length;
}

double instance__compute_length(instance_t *instance) {
  int i = 0;
  instance->length = 0.0;

  for (int i = 0; i < instance->dimension; i++) {
    int n1 = instance->tabTour[i];
    int n2 = instance->tabTour[(i + 1) % (instance->dimension)];
    double dist = instance__dist_euclidian(instance, n1, n2);
    instance->length += dist;
  }
  return instance->length;

  // while (i + 1 < instance->dimension) {
  //   instance->length += instance__dist_euclidian(instance,
  //   instance->tabTour[i],
  //                                                instance->tabTour[i +
  //                                                1]);
  //   i++;
  // }
  // instance->length += instance__dist_euclidian(
  //     instance, instance->tabTour[instance->dimension - 1],
  //     instance->tabTour[0]);

  // return instance->length;
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

void instance__set_tour(instance_t *instance, tour_t *tour) {
  assert(instance->dimension == tour->dimension);
  memcpy(instance->tabTour, tour->tour, tour->dimension * sizeof(int));
  tour->length = instance->length;
}