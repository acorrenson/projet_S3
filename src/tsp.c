#include <methods/method_bruteforce.h>
#include <methods/method_nearest_neighbour.h>
#include <tsplib/tsplib.h>

int main(int argc, char const *argv[]) {
  instance_t t;
  instance__read_from_file(&t, argv[1]);

  printf("NAME : %s\n", t.name);
  printf("TYPE : %s\n", t.type);
  printf("DATA : \n");

  for (int i = 0; i < t.dimension; i++) {
    printf("%d : (%d %d)\n", i, t.tabCoord[i][0], t.tabCoord[i][1]);
  }

  instance__compute_distances(&t);
  instance__print_matrix(&t);

  tour_t res;

  // brute_force(&t, &res);
  // tour__write_to_file(&res, stdout);
  // tour__write_graph_to_file(&res, stdout);

  // pour un affichage graphique
  // instance__set_tour(&t, &res);
  // instance__write_graph_to_file(&t, stdout, 100);

  nearest_neighbour(&t, &res);
  tour__write_to_file(&res, stdout);
  instance__write_graph_to_file(&t, stdout, 100);

  optimize_2opt(&t);
  instance__compute_length(&t);
  instance__write_graph_to_file(&t, stdout, 100);

  return 0;
}
