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

int get_shared_fragments(tour_t *t1, tour_t *t2, int ***fragments,
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

void genetic(instance_t *instance, tour_t *tour, cli_opt_t *opt) {
  tour_t *population;
  tour_t *mating_pool;
  int pop_size = 20;
  int max_generation = 10;
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