#include <cli.h>
#include <methods/method_2opt_optimisation.h>
#include <methods/method_bruteforce.h>
#include <methods/method_genetic.h>
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

/**
 * @brief Nom des méthodes de résolution.
 *
 */
const char methods[METHODS_NUMBER][BUFSIZ] = {
    "bruteforce", "bruteforce (optimisation matricielle)", "nearest-neighbour",
    "randomwalk", "genetic-algorithm"};

/**
 * @brief Ecriture des résultats complets au format CSV.
 *
 */
void write_results_as_csv(instance_t *instance, tour_t sols[METHODS_NUMBER],
                          double times[METHODS_NUMBER], cli_opt_t *opt) {
  FILE *file = opt->output_csv;
  int zero;
  if (opt->state[BAL_ZERO])
    zero = 1;
  else
    zero = 0;

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
    fprintf(file, "%6d ; %6d; %6d\n", j + zero, instance->tabCoord[j][0],
            instance->tabCoord[j][1]);
  }

  // Ecriture de la partie RESULTATS
  fprintf(file, "Méthode ; Longeur ; Temps CPU (sec) ; Tour\n");
  for (int i = 0; i < METHODS_NUMBER; i++) {
    if (sols[i].tour != NULL) {
      fprintf(file, "%s ; %.2f ; %.2f ; [", methods[i], sols[i].length,
              times[i]);
      for (int j = 0; j < sols[i].dimension - 1; j++) {
        fprintf(file, "%d, ", sols[i].tour[j] + zero);
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

  // initialisation du générateur aléatoire
  srand(time(NULL));

  // instance de problème TSP
  instance_t instance;

  // future(s) solution(s)
  tour_t res[6];

  // mesure des temps
  double exec_times[6];
  double time_start;

  // initialisation des futures tournées solutions
  for (int i = 0; i < 6; i++) {
    tour__init(&res[i]);
  }

  // ====================================
  // == Lecture des entrées
  // ====================================

  instance__read_from_file(&instance, opt.input_instance);
  instance__compute_distances(&instance);

  // === Affichage de la matrice des distances (verbose) ===
  if (opt.state[BAL_V]) {
    fprintf(opt.log, "Matrice des distances :\n");
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
      optimize_2opt(&instance, &res[SOL_NN]);
      exec_times[SOL_NN] += time(NULL) - time_start;

      // Informations (verbose)
      if (opt.state[BAL_V]) {
        fprintf(opt.log, "Optimized solution :");
        for (int i = 0; i < res[SOL_NN].dimension; i++) {
          fprintf(opt.log, "%d ", res[SOL_NN].tour[i]);
        }
        fprintf(opt.log, "(%f)\n", res[SOL_NN].length);
        ratio = (1 - res[SOL_NN].length / previous_len) * 100;
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
      optimize_2opt(&instance, &res[SOL_RW]);
      exec_times[SOL_RW] += time(NULL) - time_start;

      // Informations (verbose)
      if (opt.state[BAL_V]) {
        fprintf(opt.log, "Optimized solution :");
        for (int i = 0; i < res[SOL_RW].dimension; i++) {
          fprintf(opt.log, "%d ", res[SOL_RW].tour[i]);
        }
        fprintf(opt.log, "(%f)\n", res[SOL_RW].length);
        ratio = (1 - res[SOL_RW].length / previous_len) * 100;
        fprintf(opt.log, COLOR_G "%.2f %% " COLOR_N "optimization\n", ratio);
      }
    }
  }

  // ====================================
  // == Sorties
  // ====================================

  // === Ecriture du fichier TOUR ===
  if (opt.state[BAL_T]) {
    int i = 0;
    // recherche de la première solution
    while (i < METHODS_NUMBER && res[i].tour == NULL) {
      i++;
    }
    // écriture de la première solution
    if (i < METHODS_NUMBER) {
      tour__write_as_tsp(&res[i], opt.output_tour, !opt.state[BAL_ZERO]);
    }
  }

  // === Ecriture des résultats au format CSV ===
  write_results_as_csv(&instance, res, exec_times, &opt);

  return 0;
}
