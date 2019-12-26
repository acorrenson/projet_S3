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

/**
 * @brief Interompt l'éxecution du programme avec un message spécialisé en cas
 * d'erreur de lecture d'un fichier.
 *
 * @param filename Le nom du fichier causant l'erreur.
 */
void file_error(const char *filename) {
  fprintf(stderr,
          "[Error] while trying to opening file : " COLOR_R "%s\n" COLOR_N,
          filename);
  exit(1);
}

/**
 * @brief Avertit l'utilisateur en cas de création d'un nouveaux fichier.
 *
 * @param filename Le nom du fichier.
 */
void file_warning(const char *filename) {
  fprintf(stderr, "[Warning] creating file : " COLOR_Y "%s\n" COLOR_N,
          filename);
}

FILE *read_or_fail(const char *filename, int mode) {
  FILE *f;
  if (mode == 0) {
    f = fopen(filename, "r");
    if (f == NULL)
      file_error(filename);
  } else {
    f = fopen(filename, "w");
    file_warning(filename);
    if (f == NULL)
      file_error(filename);
  }
  return f;
}