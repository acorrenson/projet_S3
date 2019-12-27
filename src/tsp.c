#include <cli.h>
#include <methods/method_2opt_optimisation.h>
#include <methods/method_bruteforce.h>
#include <methods/method_nearest_neighbour.h>
#include <methods/method_random_walk.h>
#include <time.h>
#include <tsplib/tsplib.h>

const int METHODS_NUMBER = 5;
const int SOL_BF = 0;  // solution pour le brute_force
const int SOL_BFM = 1; // solution pour le brute_force (opti)
const int SOL_NN = 2;  // solution pour nearest neighbour
const int SOL_RW = 3;  // solution pour random walk
const int SOL_GA = 4;  // solution pour algo génétiques

const char methods[METHODS_NUMBER][BUFSIZ] = {
    "bruteforce", "bruteforce (optimisation matricielle)", "nearest-neighbour",
    "randomwalk", "genetic-algorithm"};

void write_results_as_csv(instance_t *instance, tour_t sols[METHODS_NUMBER],
                          double times[METHODS_NUMBER], cli_opt_t *opt) {
  FILE *file = opt->output_csv;
  fprintf(file, "Nom de l'instance ; %s\n", instance->name);
  if (opt->state[BAL_ZERO]) {
    fprintf(file, "Nb de ville ; %d\n", instance->dimension);
  } else {
    fprintf(file, "Nb de ville (avec (0, 0)) ; %d\n", instance->dimension);
  }
  fprintf(file, "Type ; %s\n", instance->type);
  fprintf(file, "Point ; Abs ; Ord\n");
  for (int j = 0; j < instance->dimension; j++) {
    if (opt->state[BAL_ZERO]) {
      fprintf(file, "%6d ; %6d; %6d\n", j + 1, instance->tabCoord[j][0],
              instance->tabCoord[j][1]);
    } else {
      fprintf(file, "%6d ; %6d; %6d\n", j, instance->tabCoord[j][0],
              instance->tabCoord[j][1]);
    }
  }
  fprintf(file, "Méthode ; Longeur ; Temps CPU (sec) ; Tour\n");
  for (int i = 0; i < METHODS_NUMBER; i++) {
    if (sols[i].tour != NULL) {
      fprintf(file, "%s ; %.2f ; %.2f ; [", methods[i], sols[i].length,
              times[i]);
      for (int j = 0; j < sols[i].dimension - 1; j++) {
        if (opt->state[BAL_ZERO]) {
          fprintf(file, "%d, ", sols[i].tour[j] + 1);
        } else {
          fprintf(file, "%d, ", sols[i].tour[j] + 1);
        }
      }
      fprintf(file, "%d]\n", sols[i].tour[sols[i].dimension - 1]);
    }
  }
}

int main(int argc, char const *argv[]) {
  // interprétation, des paramètres de la ligne de commande
  cli_opt_t opt;
  cli(argc, argv, &opt);

  // initialisation du générateur aléatoire
  srand(time(NULL));

  // une instance de problème TSP
  instance_t t;

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
    brute_force(&t, &res[SOL_BF], false);
    exec_times[SOL_BF] = time(NULL) - time_start;
  }

  if (opt.state[BAL_BFM]) {
    // Methode Brute Force (avec optimisation matricielle)
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Brute Force (with distance matrix)" COLOR_N
              " running...\n");
    }
    time_start = time(NULL);
    brute_force(&t, &res[SOL_BFM], true);
    exec_times[SOL_BFM] = time(NULL) - time_start;
  }

  if (opt.state[BAL_PPV]) {
    // methode Plus Proche Voisin
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Nearest Neighbour" COLOR_N " running...\n");
    }
    time_start = time(NULL);
    nearest_neighbour(&t, &res[SOL_NN]);
    exec_times[SOL_NN] = time(NULL) - time_start;
  }

  if (opt.state[BAL_RW] || (opt.state[BAL_2OPT] && !opt.state[BAL_PPV])) {
    // Marche aléatoire
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Method " COLOR_Y "Random Walk" COLOR_N " running...\n");
    }
    time_start = time(NULL);
    random_walk(&t, &res[SOL_RW]);
    exec_times[SOL_RW] = time(NULL) - time_start;
  }

  if (opt.state[BAL_2OPT]) {
    // 2 optimisation
    if (opt.state[BAL_PPV]) {
      if (opt.state[BAL_V]) {
        fprintf(opt.log,
                "Applying " COLOR_Y "2-optimization (NN)" COLOR_N "...\n");
      }
      time_start = time(NULL);
      optimize_2opt(&t, &res[SOL_NN]);
      exec_times[SOL_NN] += time(NULL) - time_start;
    }
    if (!opt.state[BAL_PPV] || opt.state[BAL_RW]) {
      if (opt.state[BAL_V]) {
        fprintf(opt.log,
                "Applying " COLOR_Y "2-optimization (RW)" COLOR_N "...\n");
      }
      time_start = time(NULL);
      optimize_2opt(&t, &res[SOL_RW]);
      exec_times[SOL_RW] += time(NULL) - time_start;
    }
  }

  write_results_as_csv(&t, res, exec_times, &opt);

  return 0;
}
