#include <cli.h>
#include <methods/method_2opt_optimisation.h>
#include <methods/method_bruteforce.h>
#include <methods/method_nearest_neighbour.h>
#include <methods/method_random_walk.h>
#include <tsplib/tsplib.h>

int main(int argc, char const *argv[]) {
  // interprétation, des paramètres de la ligne de commande
  cli_opt_t opt;
  cli(argc, argv, &opt);

  // initialisation du générateur aléatoire
  srand(time(NULL));

  // une instance de problème TSP
  instance_t t;

  const int sol_bf = 0;
  const int sol_nn = 1;
  const int sol_rw = 2;
  const int sol_ga = 3;
  const int sol_re = 4;

  // future(s) solution(s)
  tour_t res[5];

  for (int i = 0; i < 5; i++) {
    tour__init(&res[i]);
  }

  int solutions_count = 0;

  if (!opt.state[BAL_F]) {
    fprintf(opt.log, COLOR_R "[error] no input file provided !\n" COLOR_N);
    exit(1);
  }

  instance__read_from_file(&t, opt.input_instance, !opt.state[BAL_ZERO]);
  instance__compute_distances(&t);

  if (opt.state[BAL_BF]) {
    // Methode Brute Force
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Method " COLOR_Y "Brute Force" COLOR_N " running...\n");
    }
    brute_force(&t, &res[sol_bf], false);
  }

  if (opt.state[BAL_BFM]) {
    // Methode Brute Force (avec optimisation matricielle)
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Brute Force (with distance matrix)" COLOR_N
              " running...\n");
    }
    brute_force(&t, &res[sol_nn], true);
  }

  if (opt.state[BAL_PPV]) {
    // methode Plus Proche Voisin
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Nearest Neighbour" COLOR_N " running...\n");
    }
    nearest_neighbour(&t, &res[sol_nn]);
  }

  if (opt.state[BAL_RW] || (opt.state[BAL_2OPT] && !opt.state[BAL_PPV])) {
    // methode Plus Proche Voisin
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Method " COLOR_Y "Random Walk" COLOR_N " running...\n");
    }
    random_walk(&t, &res[sol_nn]);
  }

  if (opt.state[BAL_2OPT]) {
    // 2 optimisation
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Applying " COLOR_Y "2-optimization" COLOR_N "...\n");
    }
    if (opt.state[BAL_PPV]) {
      optimize_2opt(&t, &res[sol_nn]);
    } else {
      optimize_2opt(&t, &res[sol_rw]);
    }
  }

  for (int i = 0; i < 5; i++) {
    if (res[i].tour != NULL) {
      printf("solution %d\n", i);
      tour__write_to_file(&res[i], stdout);
    }
  }

  return 0;
}
