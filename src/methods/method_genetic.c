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

bool edge_in(int **edges, int edge[2], int size) {
  bool in = false;
  int i = 0;
  while (!in && i < size) {
    if ((edges[i][0] == edge[0] && edges[i][1] == edge[1]) ||
        (edges[i][0] == edge[1] && edges[i][1] == edge[0])) {
      in = true;
    }
    i++;
  }
  return in;
}

int get_shared_fragments2(tour_t *t1, tour_t *t2, int ***fragments,
                          int **sizes) {
  int **edges1;
  int **edges2;
  // calcul des arrêtes de t1
  tour__get_edges(t1, &edges1);
  // calcul des arrêtes de t2
  tour__get_edges(t2, &edges2);

  int **shared_edges = malloc((t1->dimension) * sizeof(int *));
  int n_shared_edges = 0;
  int edge[2];

  for (int i = 0; i < t1->dimension; i++) {
    shared_edges[i] = malloc(2 * sizeof(int));
  }

  // calcul des arrêtes partagées par t1 et t2
  for (int i = 0; i < t1->dimension; i++) {
    edge[0] = edges1[i][0];
    edge[1] = edges1[i][1];
    if (edge_in(edges2, edge, t1->dimension)) {
      shared_edges[n_shared_edges][0] = edges1[i][0];
      shared_edges[n_shared_edges][1] = edges1[i][1];
      n_shared_edges++;
    }
  }

  *fragments = malloc(t1->dimension * sizeof(int *));
  *sizes = malloc(t1->dimension * sizeof(int));

  for (int i = 0; i < t1->dimension; i++) {
    (*fragments)[i] = malloc(t1->dimension * sizeof(int));
    (*sizes)[i] = 0;
  }

  int ifrag = 0;
  (*fragments)[ifrag][0] = t1->tour[0];
  (*sizes)[ifrag] = 1;

  // calcul des fragments communs
  for (int i = 0; i < t1->dimension - 1; i++) {
    edge[0] = t1->tour[i];
    edge[1] = t1->tour[i + 1];
    if (edge_in(shared_edges, edge, n_shared_edges)) {
      (*fragments)[ifrag][(*sizes)[ifrag]] = edge[1];
      (*sizes)[ifrag]++;
    } else {
      ifrag++;
      (*sizes)[ifrag] = 1;
      (*fragments)[ifrag][0] = t1->tour[i + 1];
    }
  }

  return ifrag + 1;
}

int get_shared_fragments(tour_t *t1, tour_t *t2, int ***fragments,
                         int **sizes) {
  assert(t1->dimension == t2->dimension);
  // assert(tour__is_consistent(t1) && tour__is_consistent(t1));

  // == initialisation ==
  int frag_index = 0;
  int frag_size = 0;
  int dim = t1->dimension;
  int node_left = dim;
  bool *marked_1 = malloc(sizeof(bool) * dim);
  bool *marked_2 = malloc(sizeof(bool) * dim);

  //  == allocation ==
  *fragments = malloc(t1->dimension * sizeof(int *));
  *sizes = malloc(t1->dimension * sizeof(int));

  for (int i = 0; i < t1->dimension; i++) {
    (*fragments)[i] = malloc(t1->dimension * sizeof(int));
    (*sizes)[i] = 0;
    marked_1[i] = false;
    marked_2[i] = false;
  }

  int i1 = 0;
  int i2 = 0;

  // == recherche ==
  while (node_left > 0 && i1 < dim) {
    // printf("start : i1 : %d, i2 : %d\n", i1, i2);
    i2 = 0;
    while (t2->tour[i2] != t1->tour[i1]) {
      i2++;
    }

    // printf("starting with %d\n", t1->tour[i1]);
    (*fragments)[frag_index][frag_size] = t1->tour[i1];
    frag_size++;

    // (*fragments)[frag_index][0] = t1->tour[i1];
    // (*sizes)[frag_index] = 1;
    // frag_index++;
    // i1++;
    // frag_size = 0;

    if (t1->tour[i1 + 1] == t2->tour[(i2 + 1) % dim]) {
      // printf("Match (ord) bt %d - %d\n", t1->tour[i1 + 1],
      //  t2->tour[(i2 + 1) % dim]);
      i1++;
      i2++;
      (*fragments)[frag_index][frag_size] = t1->tour[i1];
      frag_size++;
      while (i1 < dim - 1 && t1->tour[i1 + 1] == t2->tour[(i2 + 1) % dim]) {
        // printf("Match (ord) bt %d - %d\n", t1->tour[i1 + 1],
        //  t2->tour[(i2 + 1) % dim]);
        i1++;
        i2++;
        (*fragments)[frag_index][frag_size] = t1->tour[i1];
        frag_size++;
      }
    } else if (t1->tour[i1 + 1] == t2->tour[(i2 - 1) % dim]) {
      // printf("Match (rev) bt %d - %d\n", t1->tour[i1 + 1],
      //  t2->tour[(i2 - 1 + dim) % dim]);
      i1++;
      i2--;
      (*fragments)[frag_index][frag_size] = t1->tour[i1];
      frag_size++;
      while (i1 < dim - 1 &&
             t1->tour[i1 + 1] == t2->tour[(i2 - 1 + dim) % dim]) {
        // printf("Match (rev) bt %d - %d\n", t1->tour[i1 + 1],
        //  t2->tour[(i2 - 1 + dim) % dim]);
        i1++;
        i2--;
        (*fragments)[frag_index][frag_size] = t1->tour[i1];
        frag_size++;
      }
    } else {
      (*fragments)[frag_index][0] = t1->tour[i1];
      frag_size = 1;
    }
    (*sizes)[frag_index] = frag_size;
    node_left = node_left - frag_size;
    printf("Found fragment : (%d)\n", frag_size);
    for (int i = 0; i < frag_size; i++) {
      printf("%d ", (*fragments)[frag_index][i]);
    }
    printf("\n");
    frag_index++;
    frag_size = 0;
    i1++;
  }

  printf("N = %d\n", frag_index);
  return frag_index;
}

int get_best(tour_t *population, int size) {
  double min_len = population[0].length;
  int imin = 0;
  for (int i = 1; i < size; i++) {
    if (population[i].length < imin) {
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
    if (population[i].length < worst_len) {
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

  int i1, i2;
  for (int generation = 0; generation < max_generation; generation++) {
    for (int i = 0; i < pop_size; i++) {
      peek_2_randomly(pop_size, &i1, &i2);
      cross_dpx(instance, &population[i1], &population[i2], &mating_pool[i]);
    }
    for (int i = 0; i < pop_size; i++) {
      tour__copy(population + i, mating_pool + i);
    }

    tour__copy(tour, &population[get_best(population, pop_size)]);

    if (opt->state[BAL_V]) {
      fprintf(opt->log, "generation  : %d\n", generation);
      fprintf(opt->log, "best tour   : " COLOR_G "%.2f\n" COLOR_N,
              population[0].length);
    }
  }
  free(mating_pool);
  free(population);
}