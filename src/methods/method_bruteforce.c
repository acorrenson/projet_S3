#include <methods/method_bruteforce.h>
#include <utils.h>

bool next_permutation(int *permut, int dim) {
  // indice de début du plus grand suffixe décroissant
  int i_suffix = dim - 1;

  // calcul de l'indice de début du plus grand suffixe décroissant
  while (i_suffix > 0 && permut[i_suffix - 1] >= permut[i_suffix]) {
    assert(i_suffix >= 0);
    i_suffix--;
    assert(i_suffix >= 0);
  }

  // si l'indice de début du plus grand suffixe décroissant est 0,
  // alors permut est dejà la plus grande permutation dans l'ordre
  // lexicographique
  if (i_suffix <= 0) {
    return false;
  }

  int i_pivot = i_suffix - 1; // indice du pivot
  int i_swap = dim - 1;       // indice de la valeur à échanger avec le pivot

  // calcul de l'indice de la valeur à échanger avec le pivot
  while (i_swap > i_suffix && permut[i_swap] <= permut[i_pivot]) {
    i_swap--;
  }

  // calcul de la permutation suivante dans l'ordre lexicographique
  swap(permut + i_swap, permut + i_pivot);
  reverse(permut, i_suffix, dim - 1);

  // la permutation en entrée n'était pas la dernière permutation dans l'ordre
  // lexicographique.
  return true;
}

void brute_force(instance_t *instance) {
  double max_dist = 0.0;

  int dim = instance->dimension - 1;

  int *tour = malloc(dim * sizeof(int));
  int *permut = malloc(dim * sizeof(int));

  for (int i = 0; i < dim; i++) {
    permut[i] = i + 1;
  }

  double dist = instance__dist_euclidian(instance, 0, permut[0]);

  int i1 = 0;
  int i2 = 1;
  while (i2 < dim) {
    dist += instance__dist_euclidian(instance, permut[i1], permut[i2]);
    i1++;
    i2++;
  }
  dist += instance__dist_euclidian(instance, permut[i2 - 1], 0);
  max_dist = dist;
  memcpy(tour, permut, dim * sizeof(int));

  while (next_permutation(permut, dim)) {
    dist = instance__dist_euclidian(instance, 0, permut[0]);
    i1 = 0;
    i2 = 1;
    while (i2 < dim) {
      dist += instance__dist_euclidian(instance, permut[i1], permut[i2]);
      i1++;
      i2++;
    }
    dist += instance__dist_euclidian(instance, permut[i2 - 1], 0);
    if (dist < max_dist) {
      max_dist = dist;
      for (int i = 0; i < dim; i++) {
        printf("%d ", permut[i]);
      }
      printf("\n");
      memcpy(tour, permut, dim * sizeof(int));
    }
  }

  for (int i = 0; i < dim; i++) {
    printf("%d ", tour[i] + 1);
  }

  printf("dist : %f\n", max_dist);

  printf("\n");
}