#include <utils.h>

void reverse(int *tab, int beg, int end) {
  assert(beg <= end);
  int i = beg;
  int j = end;
  int temp;
  while (i < j) {
    temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
    i++;
    j--;
  }
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int pivot(int *tab, int a, int b) {
  assert(a < b);
  int i_pivot = a;
  int pivot = tab[i_pivot];
  int i_end = b;
  while (i_pivot < i_end) {
    if (tab[i_pivot + 1] <= tab[i_pivot]) {
      swap(tab + i_pivot, tab + i_pivot + 1);
      i_pivot++;
    } else {
      swap(tab + i_end, tab + i_pivot + 1);
      i_end--;
    }
  }
  return i_pivot;
}

void quick_sort(int *tab, int a, int b) {
  if (a < b) {
    int i_pivot = pivot(tab, a, b);
    quick_sort(tab, a, i_pivot);
    quick_sort(tab, i_pivot + 1, b);
  }
}

FILE *read_or_fail(const char *filename, int mode) {
  FILE *f;
  if (mode == 0) {
    f = fopen(filename, "r");
    if (f == NULL) {
      fprintf(stderr,
              "Echec de l'ouverture du fichier : " COLOR_Y "%s" COLOR_N "\n",
              filename);
      exit(1);
    }
  } else {
    f = fopen(filename, "w");
    fprintf(stderr,
            COLOR_Y "[Warning] : " COLOR_N " creation du fichier " COLOR_Y
                    "%s" COLOR_N "\n",
            filename);
    if (f == NULL) {
      fprintf(stderr,
              "Echec de l'ouverture du fichier : " COLOR_Y "%s" COLOR_N "\n",
              filename);
      exit(1);
    }
  }
  return f;
}