#include <methods/method_genetic.h>

int find_fragment(int *marks, int size) {
  int i = 0;
  while (i < size && marks[i]) {
    i++;
  }
  if (i < size)
    return i;
  else
    return NIL;
}

int nearest_fragment(instance_t *inst, int node, int **fragments, int size,
                     int *sizes, int *marks, bool *reverse) {

  int f = find_fragment(marks, size);
  marks[f] = 1;
  int min_f = f;

  if (f == NIL)
    return NIL;

  double min_dist, d1, d2, d;
  bool rev;

  // printf("test %d\n", fragments[f][0]);
  d1 = instance__dist_matrix(inst, node, fragments[f][0]);
  d2 = instance__dist_matrix(inst, node, fragments[f][sizes[f] - 1]);

  if (d1 < d2) {
    min_dist = d1;
    rev = false;
  } else {
    min_dist = d2;
    rev = true;
  }

  for (int i = 0; i < size; i++) {
    if (marks[i] != 1) {
      d1 = instance__dist_euclidian(inst, node, fragments[i][0]);
      d2 = instance__dist_euclidian(inst, node, fragments[i][sizes[i] - 1]);
      if (d1 < d2) {
        d = d1;
        rev = false;
      } else {
        d = d2;
        rev = true;
      }
      if (d < min_dist) {
        min_dist = d;
        marks[min_f] = 0;
        marks[i] = 1;
        min_f = i;
        *reverse = rev;
      }
    }
  }
  return min_f;
}

void cross_dpx(instance_t *instance, tour_t *t1, tour_t *t2, tour_t *t3) {
  assert(t1->dimension == t2->dimension);
  int dim = t1->dimension;
  int **fragments; // fragments de tour
  int *sizes;      // tailles de fragments
  int *marks;      // marques sur les fragments
  int n_frag = 0;  // nombre de fragments

  n_frag = get_shared_fragments(t1, t2, &fragments, &sizes);
  marks = malloc(dim * sizeof(int));

  for (int i = 0; i < dim; i++) {
    marks[i] = 0;
  }

  tour__set_dimension(t3, dim);

  int head = fragments[0][sizes[0] - 1];
  int ihead = sizes[0] - 1;
  bool reverse;
  marks[0] = 1;

  for (int i = 0; i < sizes[0]; i++) {
    tour__add_node(t3, fragments[0][i]);
  }

  int f = nearest_fragment(instance, head, fragments, n_frag, sizes, marks,
                           &reverse);

  // jonction des fragments communs
  while (f != NIL) {
    if (reverse) {
      // il faut renverser le prochain fragment à joindre
      for (int i = sizes[f] - 1; i >= 0; i--) {
        tour__add_node(t3, fragments[f][i]);
      }
    } else {
      // on joint le prochain fragment sans le renverser
      for (int i = 0; i < sizes[f]; i++) {
        tour__add_node(t3, fragments[f][i]);
      }
    }
    // on met à jour la liste des fragments
    ihead += sizes[f];
    head = t3->tour[ihead];
    // on cherche le prochain fragment à joindre.
    f = nearest_fragment(instance, head, fragments, n_frag, sizes, marks,
                         &reverse);
  }

  // calcul de la longueur de tournée
  tour__compute_length(instance, t3, true);
}

int search_forward(tour_t *t1, tour_t *t2, int i1, int i2) {
  assert(t1->dimension == t2->dimension);
  assert(t1->tour[i1] == t2->tour[i2]);
  int size = 1;
  int dim = t1->dimension;
  while (i1 + size < dim &&
         t1->tour[i1 + size] == t2->tour[(i2 + size) % dim]) {
    size++;
  }
  return size;
}

int search_backward(tour_t *t1, tour_t *t2, int i1, int i2) {
  assert(t1->dimension == t2->dimension);
  assert(t1->tour[i1] == t2->tour[i2]);
  int size = 1;
  int dim = t1->dimension;
  while (i1 + size < dim &&
         t1->tour[i1 + size] == t2->tour[(i2 - size + dim) % dim]) {
    size++;
  }
  return size;
}

int get_shared_fragments(tour_t *t1, tour_t *t2, int ***fragments,
                         int **sizes) {

  assert(t1->dimension == t2->dimension);

  // == initialisation ==
  int frag_index = 0;
  int frag_size = 0;
  int dim = t1->dimension;
  int node_left = dim;

  //  == allocation ==
  *fragments = malloc(t1->dimension * sizeof(int *));
  *sizes = malloc(t1->dimension * sizeof(int));

  for (int i = 0; i < t1->dimension; i++) {
    (*fragments)[i] = malloc(t1->dimension * sizeof(int));
    (*sizes)[i] = 0;
  }
  int i1 = 0;
  int i2 = 0;
  int s1 = 0;
  int s2 = 0;

  // == recherche ==
  while (node_left > 0 && i1 < dim) {
    i2 = tour__index_of(t2, t1->tour[i1]);
    s1 = search_forward(t1, t2, i1, i2);
    s2 = search_backward(t1, t2, i1, i2);
    if (s1 > 1) {
      memcpy((*fragments)[frag_index], t1->tour + i1, s1 * sizeof(int));
      (*sizes)[frag_index] = s1;
      i1 = i1 + s1;
    } else if (s2 > 1) {
      memcpy((*fragments)[frag_index], t1->tour + i1, s2 * sizeof(int));
      (*sizes)[frag_index] = s2;
      i1 = i1 + s2;
    } else {
      (*fragments)[frag_index][0] = t1->tour[i1];
      (*sizes)[frag_index] = 1;
      i1++;
    }
    frag_index++;
  }

  return frag_index;
}

int get_best(tour_t *population, int size) {
  double min_len = population[0].length;
  int imin = 0;
  for (int i = 1; i < size; i++) {
    if (population[i].length < min_len) {
      min_len = population[i].length;
      imin = i;
    }
  }
  return imin;
}

void peek_2_randomly(int size, int *t1, int *t2) {
  *t1 = random() % size;
  *t2 = random() % size;
  while (*t1 == *t2) {
    *t2 = random() % size;
  }
}

int get_worst(tour_t *population, int size) {
  int worst_len = population[0].length;
  int i_worst = 0;
  for (int i = 1; i < size; i++) {
    if (population[i].length > worst_len) {
      i_worst = i;
      worst_len = population[i].length;
    }
  }
  return i_worst;
}

double average_length(tour_t *population, int size) {
  double average = 0.0;
  for (int i = 0; i < size; i++) {
    average = average + population[i].length;
  }
  return average / ((double)size);
}

void genetic(instance_t *instance, tour_t *tour, cli_opt_t *opt) {
  tour_t *population;
  tour_t *mating_pool;
  int pop_size = opt->population_size;
  int max_generation = opt->generations;
  population = malloc(pop_size * sizeof(tour_t));
  mating_pool = malloc(pop_size * sizeof(tour_t));

  srand(time(NULL));
  tour__init(tour);
  tour__set_dimension(tour, instance->dimension);

  // génération initiale
  for (int i = 0; i < pop_size; i++) {
    tour__init(population + i);
    random_walk(instance, population + i);
    // informations (verbose)
    if (opt->state[BAL_V]) {
      fprintf(opt->log,
              COLOR_N "generated tour " COLOR_Y "%3d " COLOR_N "(len:" COLOR_G
                      "% .2f" COLOR_N ") ",
              i, population[i].length);
      tour__pprint(population + i, opt->log);
    }
    instance__reset(instance);
  }

  int i1, i2, i3;
  double mutation;
  for (int generation = 0; generation < max_generation; generation++) {
    for (int i = 0; i < (int)pop_size / 3; i++) {
      i3 = get_worst(population, pop_size);
      peek_2_randomly(pop_size, &i1, &i2);
      cross_dpx(instance, &population[i1], &population[i2], &population[i3]);
      mutation = ((double)(rand() % 100)) / 100.0;
      if (mutation < opt->mutation_rate) {
        optimize_2opt(instance, &population[i3]);
      }
    }

    if (opt->state[BAL_V]) {
      fprintf(opt->log, "generation  : %d\n", generation);
      fprintf(opt->log, "best tour   : " COLOR_G "%.2f\n" COLOR_N,
              population[0].length);
    }
    tour__copy(tour, &population[get_best(population, pop_size)]);
  }
  free(population);
}