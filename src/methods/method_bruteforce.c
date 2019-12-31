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

void brute_force(instance_t *instance, tour_t *result, bool optimize) {
  int dim = instance->dimension;
  instance->tabTour = malloc(dim * sizeof(int));

  for (int i = 0; i < dim; i++) {
    instance->tabTour[i] = instance__node_at(instance, i);
  }

  tour_t best_tour;

  instance__compute_length(instance, optimize);
  instance__extract_tour(instance, &best_tour);

  while (next_permutation(instance->tabTour + 1, dim - 1)) {
    if (instance__compute_length(instance, optimize) < best_tour.length) {
      instance__extract_tour(instance, &best_tour);
    }
  }

  memcpy(result, &best_tour, sizeof(tour_t));
}
