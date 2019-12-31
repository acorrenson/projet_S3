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

  f = find_fragment(marks, size);
  marks[f] = 1;
  for (int i = 0; i < size; i++) {
    if (marks[i] != 1) {
      d1 = instance__dist_euclidian(inst, node, fragments[f][0]);
      d2 = instance__dist_euclidian(inst, node, fragments[f][sizes[f] - 1]);
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
        marks[f] = 1;
        min_f = f;
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
  int ihead = sizes[0];
  bool reverse;
  marks[0] = 1;

  for (int i = 0; i < sizes[0]; i++) {
    tour__add_node(t3, fragments[0][i]);
  }

  for (int i = 0; i < ifrag; i++) {
    for (int j = 0; j < sizes[i]; j++) {
      printf("%d ", fragments[i][j]);
    }
    printf("\n");
  }

  int f = nearest_fragment(instance, head, fragments, ifrag, sizes, marks,
                           &reverse);
  printf("Nearest of %d : ", head);
  for (int i = 0; i < sizes[f]; i++) {
    printf("%d ", fragments[f][i]);
  }
  printf("\n");
  while (f != NIL) {
    if (!reverse) {
      for (int i = sizes[f] - 1; i >= 0; i--) {
        tour__add_node(t3, fragments[f][i]);
      }
    } else {
      for (int i = 0; i < sizes[f]; i++) {
        tour__add_node(t3, fragments[f][i]);
      }
    }
    ihead += sizes[f] - 1;
    head = t3->tour[ihead];
    f = nearest_fragment(instance, head, fragments, ifrag, sizes, marks,
                         &reverse);
    printf("Nearest of %d (%d) : \n", head, ihead);
    for (int i = 0; i < sizes[f]; i++) {
      printf("%d ", fragments[f][i]);
    }
    printf("\n");
  }
}

void genetic(instance_t *instance, tour_t *tour, cli_opt_t *opt) {}