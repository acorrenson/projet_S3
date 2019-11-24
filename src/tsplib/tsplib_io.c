#include <tsplib/tsplib.h>

/**
 * @brief Exit program due to TSP file format read
 *
 * @param s
 */
void format_error(char *s) {
  fprintf(stderr,
          "Format du fichier TSB incompatible ( " COLOR_R "%s" COLOR_N " )\n",
          s);
  exit(1);
}

/**
 * @brief Print warning message in STDERR
 *
 * @param s
 */
void warning(char *s) {
  fprintf(stderr, "Warning ( " COLOR_Y "%s" COLOR_N " )\n", s);
}

/**
 * @brief Test si une chaîne commence par un motif.
 *
 * @param motif Le motif
 * @param buff  La chaine
 * @return true
 * @return false
 */
bool prefixe(char *motif, char buff[MAXBUF]) {
  size_t len_motif = strlen(motif);
  size_t len_buff = strlen(buff);

  if (len_motif <= len_buff)
    return memcmp(motif, buff, strlen(motif)) == 0;
  else
    return false;
}

void instance__read_from_file(instance_t *instance, const char *filename) {

  FILE *f = fopen(filename, "r");

  if (f == NULL) {
    fprintf(stderr,
            "Echec de l'ouverture du fichier : " COLOR_Y "%s" COLOR_N "\n",
            filename);
    exit(1);
  }

  char line[MAXBUF];
  char name[MAXBUF];
  int dim;
  int ret;
  int ville;
  int abs;
  int ord;

  // parsing du champs NAME
  fgets(line, sizeof(line), f);
  while (!prefixe("NAME", line)) {
    fgets(line, sizeof(line), f);
    if (feof(f)) {
      format_error("field NAME missing");
    }
  }
  ret = sscanf(line, "NAME : %s\n", name);
  if (ret == EOF || ret != 1) {
    format_error("Invalid field content for NAME");
  }

  // parsing du champs DIMENSION
  fgets(line, sizeof(line), f);
  while (!prefixe("DIMENSION", line)) {
    fgets(line, sizeof(line), f);
    if (feof(f)) {
      format_error("field DIMENSION missing");
    }
  }
  ret = sscanf(line, "DIMENSION : %d\n", &dim);
  if (dim <= 0) {
    format_error("DIMENSION cannot be 0");
  }
  dim++;
  if (ret == EOF || ret != 1) {
    format_error("Invalid field content for DIMENSION");
  }

  // parsing du champs DISPLAY_DATA_SECTION
  fgets(line, sizeof(line), f);
  while (!prefixe("DISPLAY_DATA_SECTION", line) &&
         !prefixe("NODE_COORD_SECTION", line)) {
    fgets(line, sizeof(line), f);
    if (feof(f)) {
      format_error("field DISPLAY_DATA_SECTION"
                   "or"
                   "NODE_COORD_SECTION missing");
    }
  }

  instance->tabCoord = malloc(dim * sizeof(int *));
  for (int i = 0; i < dim; i++) {
    instance->tabCoord[i] = malloc(4 * sizeof(int));
  }

  int *check = malloc((dim - 1) * sizeof(int));
  for (int i = 0; i < dim - 1; i++)
    check[i] = 0;

  // parsing des données
  for (int i = 0; i < dim - 1; i++) {
    fgets(line, sizeof(line), f);
    ret = sscanf(line, "%d %d %d\n", &ville, &abs, &ord);
    if (ville >= dim || ville < 0) {
      char *err = malloc(BUFSIZ);
      sprintf(err, "Point n°%d is out of dimensions", ville);
      format_error(err);
    }
    if (ville == 0) {
      format_error("Point n°0 is reserved by TSPLIB");
    }
    if (ret == EOF || ret != 3) {
      format_error("Invalid content in data section");
    } else {
      check[ville - 1] = 1;
      instance->tabCoord[ville][0] = abs;
      instance->tabCoord[ville][1] = ord;
      instance->tabCoord[ville][2] = 0;
      instance->tabCoord[ville][3] = 0;
    }
  }

  // verification des noeuds
  for (int i = 0; i < dim - 1; i++) {
    if (check[i] == 0) {
      char *err = malloc(BUFSIZ);
      sprintf(err, "point n°%d is missing", i + 1);
      format_error(err);
    }
  }

  // parsing du champs EOF
  fgets(line, sizeof(line), f);
  if (!prefixe("EOF", line)) {
    format_error("Warning : no EOF token");
  }

  // sauvegarde des données lues
  strcpy(instance->name, name);
  strcpy(instance->type, "TSP");
  instance->dimension = dim;
  instance->tabCoord[0][0] = 0;
  instance->tabCoord[0][1] = 0;
  instance->tabCoord[0][2] = 0;
  instance->tabCoord[0][3] = 0;

  fclose(f);
}

void instance__print_matrix(instance_t *instance) {
  int padd = 8;
  int prec = 3;

  printf("   ");
  for (int i = 0; i < instance->dimension; i++) {
    printf("%*d", padd, i);
  }

  printf("\n");
  for (int i = 0; i < instance->dimension * padd + 3; i++) {
    printf("-");
  }

  printf("\n");
  for (int i = 0; i < instance->dimension; i++) {
    printf("%d |", i);
    for (int j = 0; j < instance->dimension; j++) {
      if (j <= i)
        printf("%*c", padd, '.');
      else
        printf("%*.*f", padd, prec, instance->matDist[i][j]);
    }
    printf("\n");
  }
}

void tour__write_to_file(tour_t *tour, FILE *output_chan) {
  fprintf(output_chan, "NAME : %s\n", tour->name);
  fprintf(output_chan, "TYPE : TOUR\n");
  fprintf(output_chan, "DIMENSION : %d\n", tour->dimension);
  fprintf(output_chan, "LENGTH : %f\n", tour->length);
  fprintf(output_chan, "TOUR_SECTION\n");

  for (int i = 0; i < tour->dimension; i++) {
    fprintf(output_chan, "%d\n", tour->tour[i]);
  }
  fprintf(output_chan, "EOF\n");
}

void instance__write_to_file(instance_t *instance, FILE *file) {}