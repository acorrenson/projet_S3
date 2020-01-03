#include <tsplib/tsplib.h>

void instance__init(instance_t *instance, bool zero) {
  strcpy(instance->name, "NO_NAME");
  strcpy(instance->type, "NO_TYPE");
  instance->dimension = 0;
  instance->length = 0;
  instance->tabCoord = NULL;
  instance->matDist = NULL;
  instance->tabTour = NULL;
  instance->node_zero = zero;
}

void tour__init(tour_t *tour) {
  strcpy(tour->name, "NO_NAME");
  tour->length = 0;
  tour->dimension = 0;
  tour->tour = NULL;
  tour->current = 0;
}

void tour__copy(tour_t *t1, const tour_t *t2) {
  strcpy(t1->name, t2->name);
  t1->length = t2->length;
  t1->dimension = t2->dimension;
  for (int i = 0; i < t1->dimension; i++) {
    t1->tour[i] = t2->tour[i];
  }
}

double instance__dist_euclidian(instance_t *instance, int a, int b) {
  int x1 = instance->tabCoord[instance__index_of(instance, a)][0];
  int y1 = instance->tabCoord[instance__index_of(instance, a)][1];
  int x2 = instance->tabCoord[instance__index_of(instance, b)][0];
  int y2 = instance->tabCoord[instance__index_of(instance, b)][1];

  double r = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double instance__dist_matrix(instance_t *instance, int a, int b) {

  // on utilise une demie matrice SUPERIEURE
  if (a < b)
    return instance->matDist[instance__index_of(instance, a)]
                            [instance__index_of(instance, b)];
  else
    return instance->matDist[instance__index_of(instance, b)]
                            [instance__index_of(instance, a)];
}

void instance__compute_distances(instance_t *instance) {
  instance->matDist = malloc(instance->dimension * sizeof(double *));
  for (int i = 0; i < instance->dimension; i++) {
    instance->matDist[i] = malloc(instance->dimension * sizeof(double));
  }

  for (int i = 0; i < instance->dimension; i++) {
    for (int j = i + 1; j < instance->dimension; j++) {
      instance->matDist[i][j] =
          instance__dist_euclidian(instance, instance__node_at(instance, i),
                                   instance__node_at(instance, j));
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
  t->current = 0;
  t->tour = malloc(dim * sizeof(int));
}

void tour__add_node(tour_t *t, int node) {
  t->tour[t->current] = node;
  t->current++;
}

bool tour__has_node(tour_t *t, int node) {
  int i = 0;
  while (i < t->dimension && t->tour[i] != node) {
    i++;
  }
  if (i >= t->dimension)
    return false;
  else
    return true;
}

double tour__compute_length(instance_t *instance, tour_t *tour, bool optimize) {
  assert(tour->dimension == instance->dimension);
  int i = 0;
  tour->length = 0.0;

  for (int i = 0; i < instance->dimension; i++) {
    int n1 = tour->tour[i];
    int n2 = tour->tour[(i + 1) % (instance->dimension)];
    double dist;
    if (optimize) {
      dist = instance__dist_matrix(instance, n1, n2);
    } else {
      dist = instance__dist_euclidian(instance, n1, n2);
    }
    tour->length += dist;
  }
  return tour->length;
}

double instance__compute_length(instance_t *instance, bool optimize) {
  int i = 0;
  instance->length = 0.0;

  for (int i = 0; i < instance->dimension; i++) {
    int n1 = instance->tabTour[i];
    int n2 = instance->tabTour[(i + 1) % (instance->dimension)];
    double dist;
    if (optimize) {
      dist = instance__dist_matrix(instance, n1, n2);
    } else {
      dist = instance__dist_euclidian(instance, n1, n2);
    }
    instance->length += dist;
  }
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

void instance__mark(instance_t *instance, int node) {
  instance->tabCoord[instance__index_of(instance, node)][2] = 1;
}

void instance__unmark(instance_t *instance, int node) {
  instance->tabCoord[instance__index_of(instance, node)][2] = 0;
}

bool instance__marked(instance_t *instance, int node) {
  if (instance->tabCoord[instance__index_of(instance, node)][2] == 0) {
    return false;
  } else {
    return true;
  }
}

int instance__find_non_marked(instance_t *instance) {
  for (int i = 0; i < instance->dimension; i++) {
    if (!instance__marked(instance, instance__node_at(instance, i)))
      return instance__node_at(instance, i);
  }
  return NIL;
}

int instance__index_of(instance_t *inst, int node) {
  if (inst->node_zero) {
    return node;
  } else {
    return node - 1;
  }
}

int instance__node_at(instance_t *inst, int index) {
  if (inst->node_zero) {
    return index;
  } else {
    return index + 1;
  }
}

void tour__get_edges(tour_t *t, int ***edges) {
  *edges = malloc(t->dimension * sizeof(int *));
  for (int i = 0; i < t->dimension; i++) {
    (*edges)[i] = malloc(2 * sizeof(int));
    if (t->tour[i] < t->tour[(i + 1) % t->dimension]) {
      (*edges)[i][0] = t->tour[i];
      (*edges)[i][1] = t->tour[(i + 1) % t->dimension];
    } else {
      (*edges)[i][0] = t->tour[(i + 1) % t->dimension];
      (*edges)[i][1] = t->tour[i];
    }
  }
}