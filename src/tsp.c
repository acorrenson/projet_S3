#include <methods/method_2opt_optimisation.h>
#include <methods/method_bruteforce.h>
#include <methods/method_nearest_neighbour.h>
#include <methods/method_random_walk.h>
#include <tsplib/tsplib.h>

int main(int argc, char const *argv[]) {

  srand(time(NULL));
  // une instance de problème TSP
  instance_t t;
  instance__read_from_file(&t, argv[1], true);
  instance__compute_distances(&t);

  // futur solution
  tour_t res;

  // methode Plus Proche Voisin
  nearest_neighbour(&t, &res);
  printf("\nSolution avec methode PPV: \n");
  tour__write_to_file(&res, stdout);
  // tour__save_to_csv(&t, &res, stdout);

  // optimisation 2
  optimize_2opt(&t, &res);
  printf("\nSolution avec methode PPV + 2-optmisation : \n");
  tour__write_to_file(&res, stdout);
  // tour__save_to_csv(&t, &res, stdout);

  // bruteforce
  instance__reset(&t);
  brute_force(&t, &res);
  printf("\nSolution avec methode BF : \n");
  tour__write_to_file(&res, stdout);
  tour__save_to_csv(&t, &res, stdout);

  // random walk
  instance__reset(&t);
  random_walk(&t, &res);
  printf("\nSolution avec methode RW : \n");
  tour__write_to_file(&res, stdout);
  // tour__save_to_csv(&t, &res, stdout);

  return 0;
}
