#include <methods/method_2opt_optimisation.h>
#include <methods/method_bruteforce.h>
#include <methods/method_nearest_neighbour.h>
#include <tsplib/tsplib.h>

int main(int argc, char const *argv[]) {

  srand(time(NULL));
  // une instance de problème TSP
  instance_t t;
  instance__read_from_file(&t, argv[1]);

  // futur solution
  tour_t res;

  // methode Plus Proche Voisin
  nearest_neighbour(&t, &res);
  printf("\nSolution avec methode PPV: \n");
  tour__write_to_file(&res, stdout);
  tour__write_coords_to_file(&t, &res, stdout);

  optimize_2opt(&t, &res);
  printf("\nSolution avec methode PPV + 2-optmisation : \n");
  tour__write_to_file(&res, stdout);
  tour__write_coords_to_file(&t, &res, stdout);

  brute_force(&t, &res);
  printf("\nSolution avec methode BF : \n");
  tour__write_to_file(&res, stdout);
  tour__write_coords_to_file(&t, &res, stdout);

  instance__reset(&t);
  random_walk(&t, &res);
  printf("\nSolution avec methode RW : \n");
  tour__write_to_file(&res, stdout);

  return 0;
}
