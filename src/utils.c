#include <utils.h>

void reverse(int *tab, int dim) {
  assert(dim >= 0);
  int i = 0;
  int j = dim - 1;
  int temp;
  while (i < j) {
    temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
    i++;
    j--;
  }
}