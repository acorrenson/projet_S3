#include <cli.h>

void cli_opt__init(cli_opt_t *opt) {
  opt->input_instance = stdin;
  opt->output_tour = NULL;
  opt->output_csv = stdout;
  opt->log = stderr;
  for (int i = 0; i < BAL_COUNT; i++) {
    opt->state[i] = false;
  }
  opt->population_size = 20;
  opt->generations = 200;
  opt->mutation_rate = 0.3;
}

void help() {
  fprintf(
      stderr,
      "Usage : ./tsp -f <file> [-t <tour>] [-v [<file>]] -<méthode> "
      "[-h]\n" COLOR_G "-f" COLOR_N
      " <file> : nom du fichier tsp (obligatoire)\n" COLOR_G "-t" COLOR_N
      " <file>" COLOR_N " : nom du fichier solution (optionnel)\n" COLOR_G
      "-v" COLOR_N " [<file>] : mode verbose (optionnel), écrit dans un "
      "fichier si présent\n" COLOR_G "-o" COLOR_N
      " <file> : export des résultats dans un fichier csv\n" COLOR_G
      "-h" COLOR_N " : help, affiche ce texte\n"
      "\n<méthode> : méthodes de calcul (cumulables) :\n\n" COLOR_G
      "-bf" COLOR_N " : brute force,\n" COLOR_G "-bfm" COLOR_N
      " : brute force en utilisant la matrice de distance,\n" COLOR_G
      "-ppv" COLOR_N " : plus proche voisin,\n" COLOR_G "-rw" COLOR_N
      " : random walk,\n" COLOR_G "-2opt" COLOR_N
      " : 2 optimisation. Si -ppv ou -rw ne sont pas présentes on "
      "utilise "
      "-rw,\n" COLOR_G "-ga" COLOR_N
      " <nombre d'individus> <nombre de générations> <taux de "
      "mutation>"
      " : algorithme génétique, défaut = 20 individus, 200 générations, 0.3 "
      "mutation.\n");
}

void cli(int argc, char const *argv[], cli_opt_t *opt) {

  cli_opt__init(opt);
  int i = 1;
  int methods = 0;
  while (i < argc) {

    // Balise -f
    if (strcmp("-f", argv[i]) == 0) {
      opt->state[BAL_F] = true;
      if (i + 1 < argc) {
        i++;
        opt->input_instance = read_or_fail(argv[i], 0);
      } else {
        fprintf(stderr, COLOR_R
                "[cli - error] file expected after flag (-f)\n" COLOR_N);
        exit(1);
      }
    }

    // Balise -t
    else if (strcmp("-t", argv[i]) == 0) {
      opt->state[BAL_T] = true;
      if (i + 1 < argc) {
        i++;
        opt->output_tour = read_or_fail(argv[i], 1);
      } else {
        fprintf(stderr, COLOR_R
                "[cli - error] file expected after flag (-t)\n" COLOR_N);
        exit(1);
      }
    }

    // Balise -v
    else if (strcmp("-v", argv[i]) == 0) {
      opt->state[BAL_V] = true;
      if (i + 1 < argc && argv[i + 1][0] != '-') {
        i++;
        opt->log = read_or_fail(argv[i], 1);
      }
    }

    // Balise -o
    else if (strcmp("-o", argv[i]) == 0) {
      opt->state[BAL_O] = true;
      if (i + 1 < argc) {
        i++;
        opt->output_csv = read_or_fail(argv[i], 1);
      } else {
        fprintf(stderr, COLOR_R
                "[cli - error] file expected after flag (-o)\n" COLOR_N);
        exit(1);
      }
    }

    // Balise -ppv
    else if (strcmp("-ppv", argv[i]) == 0) {
      opt->state[BAL_PPV] = true;
      methods++;
    }

    // Balise -rw
    else if (strcmp("-rw", argv[i]) == 0) {
      opt->state[BAL_RW] = true;
      methods++;
    }

    // Balise -2opt
    else if (strcmp("-2opt", argv[i]) == 0) {
      opt->state[BAL_2OPT] = true;
      methods++;
    }

    // Balise -bf
    else if (strcmp("-bf", argv[i]) == 0) {
      opt->state[BAL_BF] = true;
      methods++;
    }

    // Balise -bfm
    else if (strcmp("-bfm", argv[i]) == 0) {
      opt->state[BAL_BFM] = true;
      methods++;
    }

    // Balise -ga
    else if (strcmp("-ga", argv[i]) == 0) {
      opt->state[BAL_GA] = true;
      methods++;
      if (i + 3 < argc) {
        if (argv[i + 1][0] != '-' && argv[i + 2][0] != '-' &&
            argv[i + 3][0] != '-') {
          if (sscanf(argv[i + 1], "%d", &opt->population_size) < 1) {
            fprintf(stderr, COLOR_R
                    "[cli - error] bad data format after flag (-ga)\n" COLOR_N);
            exit(1);
          }
          if (sscanf(argv[i + 2], "%d", &opt->generations) < 1) {
            fprintf(stderr, COLOR_R
                    "[cli - error] bad data format after flag (-ga)\n" COLOR_N);
            exit(1);
          }
          if (sscanf(argv[i + 3], "%lf", &opt->mutation_rate) < 1) {
            fprintf(stderr, COLOR_R
                    "[cli - error] bad data format after flag (-ga)\n" COLOR_N);
            exit(1);
          }
          i = i + 3;
          printf("genetic :\n");
          printf("%d %d %lf\n", opt->population_size, opt->generations,
                 opt->mutation_rate);
        }
      }
    }

    // Balise -nz
    else if (strcmp("-nz", argv[i]) == 0) {
      opt->state[BAL_ZERO] = true;
    }

    // Balise -h
    else if (strcmp("-h", argv[i]) == 0) {
      opt->state[BAL_H] = true;
    }

    // Balise inconnue
    else {
      fprintf(stderr, COLOR_R "[cli - error] invalid option %s\n" COLOR_N,
              argv[i]);
      exit(1);
    }
    i++;
  }

  if (!opt->state[BAL_F] && !opt->state[BAL_H]) {
    fprintf(stderr, COLOR_R "[cli - error] no input file provided\n" COLOR_N);
    exit(1);
  }

  if (methods == 0 && !opt->state[BAL_H]) {
    fprintf(stderr, COLOR_R "[cli - error] no method provided\n" COLOR_N);
    exit(1);
  }
}