#include <cli.h>
#include <methods/method_2opt_optimisation.h>
#include <methods/method_bruteforce.h>
#include <methods/method_genetic.h>
#include <methods/method_nearest_neighbour.h>
#include <methods/method_random_walk.h>
#include <time.h>
#include <tsplib/tsplib.h>

#define METHODS_NUMBER 7

const int SOL_BF = 0;   // solution pour le brute_force
const int SOL_BFM = 1;  // solution pour le brute_force (opti)
const int SOL_NN = 2;   // solution pour nearest neighbour
const int SOL_RW = 3;   // solution pour random walk
const int SOL_GA = 4;   // solution pour algo génétiques
const int SOL_RW_O = 5; // solution pour random walk
const int SOL_NN_O = 6; // solution pour random walk

/**
 * @brief Nom des méthodes de résolution.
 *
 */
const char methods[METHODS_NUMBER][BUFSIZ] = {
    "bruteforce",
    "bruteforce (optimisation matricielle)",
    "nearest-neighbour",
    "randomwalk",
    "genetic-algorithm",
    "randomwalk (2opt)",
    "nearest-neighbour (2opt)"};

/**
 * @brief Ecriture des résultats complets au format CSV.
 *
 */
void write_results_as_csv(instance_t *instance, tour_t sols[METHODS_NUMBER],
                          double times[METHODS_NUMBER], cli_opt_t *opt) {
  FILE *file = opt->output_csv;

  // Ecriture de la partie INSTANCE
  fprintf(file, "Nom de l'instance ; %s\n", instance->name);
  if (opt->state[BAL_ZERO]) {
    fprintf(file, "Nb de ville ; %d\n", instance->dimension);
  } else {
    fprintf(file, "Nb de ville (avec (0, 0)) ; %d\n", instance->dimension);
  }
  fprintf(file, "Type ; %s\n", instance->type);
  fprintf(file, "Point ; Abs ; Ord\n");
  for (int j = 0; j < instance->dimension; j++) {
    fprintf(file, "%6d ; %6d; %6d\n", instance__node_at(instance, j),
            instance->tabCoord[j][0], instance->tabCoord[j][1]);
  }

  // Ecriture de la partie RESULTATS
  fprintf(file, "Méthode ; Longeur ; Temps CPU (sec) ; Tour\n");
  for (int i = 0; i < METHODS_NUMBER; i++) {
    if (sols[i].tour != NULL) {
      fprintf(file, "%s ; %.2f ; %.2f ; [", methods[i], sols[i].length,
              times[i]);
      for (int j = 0; j < sols[i].dimension - 1; j++) {
        fprintf(file, "%d, ", sols[i].tour[j]);
      }
      fprintf(file, "%d]\n", sols[i].tour[sols[i].dimension - 1]);
    }
  }
}

int main(int argc, char const *argv[]) {
  // ====================================
  // == Initialisation
  // ====================================

  // interprétation, des paramètres de la ligne de commande
  cli_opt_t opt;
  cli(argc, argv, &opt);

  // affichage de l'aide
  if (opt.state[BAL_H]) {
    help();
    exit(1);
  }

  // initialisation du générateur aléatoire
  srand(time(NULL));

  // instance de problème TSP
  instance_t instance;

  // future(s) solution(s)
  tour_t res[METHODS_NUMBER];

  // mesure des temps
  double exec_times[METHODS_NUMBER];
  double time_start;

  // initialisation des futures tournées solutions
  for (int i = 0; i < 6; i++) {
    tour__init(&res[i]);
  }

  // ====================================
  // == Lecture des entrées
  // ====================================

  instance__init(&instance, !opt.state[BAL_ZERO]);
  instance__read_from_file(&instance, opt.input_instance);
  instance__compute_distances(&instance);

  // === Affichage de la matrice des distances (verbose) ===
  if (opt.state[BAL_V]) {
    fprintf(opt.log, "Distances matrix :\n");
    instance__print_matrix(&instance);
  }

  // ====================================
  // == Application des méthodes
  // ====================================

  // === Methode Brute Force ===
  if (opt.state[BAL_BF]) {
    // Informations (verbose)
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Method " COLOR_Y "Brute Force" COLOR_N " running...\n");
    }
    // calcul
    time_start = time(NULL);
    brute_force(&instance, &res[SOL_BF], false);
    exec_times[SOL_BF] = time(NULL) - time_start;
  }

  // === Methode Brute Force (matrice) ===
  if (opt.state[BAL_BFM]) {
    // Informations (verbose)
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Brute Force (with distance matrix)" COLOR_N
              " running...\n");
    }
    // calcul
    time_start = time(NULL);
    brute_force(&instance, &res[SOL_BFM], true);
    exec_times[SOL_BFM] = time(NULL) - time_start;
  }

  // === Methode Plus Proches Voisins ===
  if (opt.state[BAL_PPV]) {
    // Informations (verbose)
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Nearest Neighbour" COLOR_N " running...\n");
    }
    // calcul
    time_start = time(NULL);
    nearest_neighbour(&instance, &res[SOL_NN]);
    exec_times[SOL_NN] = time(NULL) - time_start;
  }

  // === Méthode Marche Aléatoire ===
  if (opt.state[BAL_RW] || (opt.state[BAL_2OPT] && !opt.state[BAL_PPV])) {
    // Informations (verbose)
    if (opt.state[BAL_V]) {
      fprintf(opt.log, "Method " COLOR_Y "Random Walk" COLOR_N " running...\n");
    }
    // calcul
    time_start = time(NULL);
    random_walk(&instance, &res[SOL_RW]);
    exec_times[SOL_RW] = time(NULL) - time_start;
  }

  // === Optimisations ===
  if (opt.state[BAL_2OPT]) {
    double previous_len;
    double ratio;

    // => Optimisations de la méthode Plus Proches Voisins
    if (opt.state[BAL_PPV]) {
      // Informations (verbose)
      if (opt.state[BAL_V]) {
        fprintf(opt.log, "Applying " COLOR_Y
                         "2-optimization (nearest-neighbour)" COLOR_N "...\n");
        fprintf(opt.log, "Initial solution :");
        for (int i = 0; i < res[SOL_NN].dimension; i++) {
          fprintf(opt.log, "%d ", res[SOL_NN].tour[i]);
        }
        fprintf(opt.log, "(%f)\n", res[SOL_NN].length);
      }

      // calcul
      time_start = time(NULL);
      previous_len = res[SOL_NN].length;
      tour__set_dimension(&res[SOL_NN_O], res[SOL_NN].dimension);
      tour__copy(&res[SOL_NN_O], &res[SOL_NN]);
      optimize_2opt_full(&instance, &res[SOL_NN_O]);
      exec_times[SOL_NN_O] = exec_times[SOL_NN] + time(NULL) - time_start;

      // Informations (verbose)
      if (opt.state[BAL_V]) {
        fprintf(opt.log, "Optimized solution :");
        for (int i = 0; i < res[SOL_NN].dimension; i++) {
          fprintf(opt.log, "%d ", res[SOL_NN_O].tour[i]);
        }
        fprintf(opt.log, "(%f)\n", res[SOL_NN_O].length);
        ratio = (1 - res[SOL_NN_O].length / previous_len) * 100;
        fprintf(opt.log, COLOR_G "%.2f %% " COLOR_N "optimization\n", ratio);
      }
    }

    // => Optimisations de la méthode Marche Aléatoire
    if (!opt.state[BAL_PPV] || opt.state[BAL_RW]) {
      // Informations (verbose)
      if (opt.state[BAL_V]) {
        fprintf(opt.log, "Applying " COLOR_Y
                         "2-optimization (random-walk)" COLOR_N "...\n");
        fprintf(opt.log, "Initial solution :");
        for (int i = 0; i < res[SOL_RW].dimension; i++) {
          fprintf(opt.log, "%d ", res[SOL_RW].tour[i]);
        }
        fprintf(opt.log, "(%f)\n", res[SOL_RW].length);
      }

      // calcul
      time_start = time(NULL);
      previous_len = res[SOL_RW].length;
      tour__set_dimension(&res[SOL_RW_O], res[SOL_RW].dimension);
      tour__copy(&res[SOL_RW_O], &res[SOL_RW]);
      optimize_2opt_full(&instance, &res[SOL_RW_O]);
      exec_times[SOL_RW_O] = exec_times[SOL_RW] + time(NULL) - time_start;

      // Informations (verbose)
      if (opt.state[BAL_V]) {
        fprintf(opt.log, "Optimized solution :");
        for (int i = 0; i < res[SOL_RW_O].dimension; i++) {
          fprintf(opt.log, "%d ", res[SOL_RW_O].tour[i]);
        }
        fprintf(opt.log, "(%f)\n", res[SOL_RW_O].length);
        ratio = (1 - res[SOL_RW_O].length / previous_len) * 100;
        fprintf(opt.log, COLOR_G "%.2f %% " COLOR_N "optimization\n", ratio);
      }
    }
  }

  // === Methode Algo Genetique ===
  if (opt.state[BAL_GA]) {
    if (opt.state[BAL_V]) {
      fprintf(opt.log,
              "Method " COLOR_Y "Genetic Algorithm" COLOR_N " running...\n");
    }
    // calcul
    time_start = time(NULL);
    genetic(&instance, &res[SOL_GA], &opt);
    exec_times[SOL_GA] = time(NULL) - time_start;
  }

  // ====================================
  // == Sorties
  // ====================================

  double best_len;
  int best_i = 0;
  while (best_i < METHODS_NUMBER && res[best_i].tour == NULL) {
    best_i++;
  }
  best_len = res[best_i].length;

  // === Ecriture du fichier TOUR ===
  if (opt.state[BAL_T]) {
    int i = 0;
    // recherche de la meilleure solution
    for (int i = 0; i < METHODS_NUMBER; i++) {
      if (res[i].tour != NULL && res[i].length < best_len) {
        best_len = res[i].length;
        best_i = i;
      }
    }
    // écriture de la meilleure solution
    tour__write_as_tsp(&res[i], opt.output_tour);
  }

  // === Ecriture des résultats au format CSV ===
  write_results_as_csv(&instance, res, exec_times, &opt);

  return 0;
}
