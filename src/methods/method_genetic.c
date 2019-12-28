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
                     int *sizes, int *marks) {

  int f = find_fragment(marks, size);
  int min_f = f;

  if (f == NIL)
    return NIL;

  double min_dist, d1, d2, d;
  d1 = instance__dist_euclidian(inst, node, fragments[f][0]);
  d2 = instance__dist_euclidian(inst, node, fragments[f][sizes[f] - 1]);

  if (d1 < d2) {
    min_dist = d1;
  } else {
    min_dist = d2;
  }

  f = find_fragment(marks, size);
  while (f != NIL) {
    d1 = instance__dist_euclidian(inst, node, fragments[f][0]);
    d2 = instance__dist_euclidian(inst, node, fragments[f][sizes[f] - 1]);
    if (d1 < d2)
      d = d1;
    else
      d = d2;
    if (d < min_dist) {
      min_dist = d;
      min_f = f;
    }
  }
  marks[min_f] = 1;
  return min_f;
}

void cross_dpx(instance_t *instance, tour_t *t1, tour_t *t2, tour_t *t3) {
  assert(t1->dimension == t2->dimension);
  int dim = t1->dimension;
  int **fragments; // fragments de tour
  int *sizes;      // tailles de fragments
  int *marks;      // marques sur les fragments
  int ifrag = 0;   // indice de fragment

  fragments = malloc(dim * sizeof(int));
  sizes = malloc(dim * sizeof(int));
  marks = malloc(dim * sizeof(int));

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

  tour_t res;
  res.dimension = dim;
  res.current = 0;
  res.tour = malloc(dim * sizeof(int));

  int head = fragments[0][sizes[0] - 1];
  int k = sizes[0];
  marks[0] = 1;
  int f = nearest_fragment(instance, head, fragments, ifrag, sizes, marks);
  while (f != NIL) {
    printf("chain %d\n", f);
  }
}

void genetic(instance_t *instance, tour_t *tour, cli_opt_t *opt) {}