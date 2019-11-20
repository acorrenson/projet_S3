#include <stdio.h>
#include <stdlib.h>

void print_matrix(double **mat, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%3.2d", mat[i][j]);
    }
    printf("\n");
  }
}