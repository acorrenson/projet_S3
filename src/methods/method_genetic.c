#include <methods/method_genetic.h>
#include <methods/method_nearest_neighbour.h>

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

  d1 = instance__dist_euclidian(inst, node, fragments[f][0]);
  d2 = instance__dist_euclidian(inst, node, fragments[f][sizes[f] - 1]);

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
  int ifrag = 0;   // indice de fragment

  fragments = malloc(dim * sizeof(int *));
  sizes = malloc((dim + 2) * sizeof(int));
  marks = malloc(dim * sizeof(int));

  assert(sizes != NULL);

  for (int i = 0; i < dim; i++) {
    marks[i] = 0;
  }

  int i1 = 0;   // indice dans le tour 1
  int i2 = 0;   // indice dans le tour 2
  int size = 0; // taille du fragment courrant

  while (i1 < dim) {
    assert(tour__has_node(t2, t1->tour[i1]));
    assert(dim >= 2);
    size = 0;
    // trouver la première occurence de t1->tour[i1] dans t2
    while (t1->tour[i1] != t2->tour[i2 % dim]) {
      i2++;
    }
    // trouver le plus long fragment commun partant de t1->tour[i1]
    while (t1->tour[i1] == t2->tour[i2 % dim] && i1 < dim) {
      size++;
      i1++;
      i2++;
    }
    fragments[ifrag] = malloc(size * sizeof(int));
    sizes[ifrag] = size;
    for (int i = 0; i < size; i++) {
      fragments[ifrag][i] = t1->tour[(i1 - size + i) % dim];
    }
    ifrag++;
  }

  t3->dimension = dim;
  t3->current = 0;
  t3->tour = malloc(dim * sizeof(int));

  int head = fragments[0][sizes[0] - 1];
  int ihead = sizes[0] - 1;
  bool reverse;
  marks[0] = 1;

  for (int i = 0; i < sizes[0]; i++) {
    tour__add_node(t3, fragments[0][i]);
  }

  int f = nearest_fragment(instance, head, fragments, ifrag, sizes, marks,
                           &reverse);

  while (f != NIL) {
    if (reverse) {
      for (int i = sizes[f] - 1; i >= 0; i--) {
        tour__add_node(t3, fragments[f][i]);
      }
    } else {
      for (int i = 0; i < sizes[f]; i++) {
        tour__add_node(t3, fragments[f][i]);
      }
    }
    ihead += sizes[f];
    head = t3->tour[ihead];
    f = nearest_fragment(instance, head, fragments, ifrag, sizes, marks,
                         &reverse);
  }
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

int explode(tour_t *t1, tour_t *t2, int ***fragments, int **sizes) {
  int **edges1;
  int **edges2;
  // calcul des arrêtes de t1
  tour__get_edges(t1, &edges1);
  // calcul des arrêtes de t2
  tour__get_edges(t2, &edges2);

  int **shared_edges = malloc((t1->dimension - 1) * sizeof(int *));
  int n_shared_edges = 0;
  int edge[2];

  for (int i = 0; i < t1->dimension - 1; i++) {
    shared_edges[i] = malloc(2 * sizeof(int));
  }

  // calcul des arrêtes partagées par t1 et t2
  for (int i = 0; i < t1->dimension - 1; i++) {
    edge[0] = edges1[i][0];
    edge[1] = edges1[i][1];
    if (edge_in(edges2, edge, t1->dimension - 1)) {
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

void genetic(instance_t *instance, tour_t *tour, cli_opt_t *opt) {}