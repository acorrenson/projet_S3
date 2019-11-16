#include <tspstat.h>

int main(int argc, char const *argv[]) {
  instance_t t;
  lecture_fichier(argv[1], &t);

  printf("NAME : %s\n", t.name);
  printf("TYPE : %s\n", t.type);
  printf("DATA : \n");
  
  for (int i = 0; i < t.dimension; i++) {
    printf("%d : (%d %d)\n", i, t.tabCoord[i][0], t.tabCoord[i][1]);
  }
  
  return 0;
}
