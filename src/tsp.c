#include <cli.h>
#include <methods/method_2opt_optimisation.h>
#include <methods/method_bruteforce.h>
#include <methods/method_nearest_neighbour.h>
#include <methods/method_random_walk.h>
#include <time.h>
#include <tsplib/tsplib.h>

int main(int argc, char const *argv[]) {
  // interprétation, des paramètres de la ligne de commande
  cli_opt_t opt;
  cli(argc, argv, &opt);

  // initialisation du générateur aléatoire
  srand(time(NULL));

  // une instance de problème TSP
  instance_t t;

  const int sol_bf = 0;  // solution pour le brute_force
  const int sol_bfm = 1; // solution pour le brute_force (opti)
  const int sol_nn = 2;  // solution pour nearest neighbour
  const int sol_rw = 3;  // solution pour random walk
  const int sol_ga = 4;  // solution pour algo génétiques
  const int sol_re = 5;  // solution pour recuit simulé

  // future(s) solution(s)
  tour_t res[6];
  // mesure des temps
  double exec_times[6];
  double time_start;

  for (int i = 0; i < 6; i++) {
    tour__init(&res[i]);
  }

  if (!opt.state[BAL_F]) {
    fprintf(opt.log, COLOR_R "[Error] no input file provided !\n" COLOR_N);
    exit(1);
  }

  instance__read_from_file(&t, opt.input_instance, !opt.state[BAL_ZERO]);
  instance__compute_distances(&t);

  if (opt.state[BAL_BF]) {
    // Methode Brute Force
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Method " COLOR_Y "Brute Force" COLOR_N " running...\n");
    }
    time_start = time(NULL);
    brute_force(&t, &res[sol_bf], false);
    exec_times[sol_bf] = time(NULL) - time_start;
  }

  if (opt.state[BAL_BFM]) {
    // Methode Brute Force (avec optimisation matricielle)
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Brute Force (with distance matrix)" COLOR_N
              " running...\n");
    }
    time_start = time(NULL);
    brute_force(&t, &res[sol_bfm], true);
    exec_times[sol_bfm] = time(NULL) - time_start;
  }

  if (opt.state[BAL_PPV]) {
    // methode Plus Proche Voisin
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Nearest Neighbour" COLOR_N " running...\n");
    }
    time_start = time(NULL);
    nearest_neighbour(&t, &res[sol_nn]);
    exec_times[sol_nn] = time(NULL) - time_start;
  }

  if (opt.state[BAL_RW] || (opt.state[BAL_2OPT] && !opt.state[BAL_PPV])) {
    // Marche aléatoire
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Method " COLOR_Y "Random Walk" COLOR_N " running...\n");
    }
    time_start = time(NULL);
    random_walk(&t, &res[sol_rw]);
    exec_times[sol_rw] = time(NULL) - time_start;
  }

  if (opt.state[BAL_2OPT]) {
    // 2 optimisation
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Applying " COLOR_Y "2-optimization" COLOR_N "...\n");
    }
    if (opt.state[BAL_PPV]) {
      time_start = time(NULL);
      optimize_2opt(&t, &res[sol_nn]);
      exec_times[sol_nn] += time(NULL) - time_start;
    } else {
      time_start = time(NULL);
      optimize_2opt(&t, &res[sol_rw]);
      exec_times[sol_rw] += time(NULL) - time_start;
    }
  }

  for (int i = 0; i < 5; i++) {
    if (res[i].tour != NULL) {
      printf("solution %d [ %f ]\n", i, exec_times[i]);
      tour__write_as_tsp(&res[i], stdout, !opt.state[BAL_ZERO]);
    }
  }
  return 0;
}
