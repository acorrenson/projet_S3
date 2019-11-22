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

void instance__print_matrix(instance_t *instance) {

  int padd = 8;
  int prec = 3;

  printf("   ");
  for (int i = 0; i < instance->dimension; i++) {
    printf("%*d", padd, i);
  }

  printf("\n");
  for (int i = 0; i < instance->dimension * padd + 3; i++) {
    printf("-");
  }

  printf("\n");
  for (int i = 0; i < instance->dimension; i++) {
    printf("%d |", i);
    for (int j = 0; j < instance->dimension; j++) {
      if (j <= i)
        printf("%*c", padd, '.');
      else
        printf("%*.*f", padd, prec, instance->matDist[i][j]);
    }
    printf("\n");
  }
}