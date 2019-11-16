#include "pretty.h"
#include "tspstat.h"

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

/**
 * @brief Lit un fichier au format TSP.
 *
 * @param filename  Le nom du fichier TSP.
 * @param instante  Pointeur sur l'instance à initialiser
 * @return int      1 en cas d'erreur(s), 0 sinon.
 */
int lecture_fichier(const char *filename, instance_t *instance) {

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

  // parsing des données
  for (int i = 0; i < dim; i++) {
    fgets(line, sizeof(line), f);
    ret = sscanf(line, "%d %d %d\n", &ville, &abs, &ord);
    if (ret == EOF || ret != 3) {
      format_error("Invalid content in data section");
    } else {
      instance->tabCoord[ville][0] = abs;
      instance->tabCoord[ville][1] = ord;
      instance->tabCoord[ville][2] = 0;
      instance->tabCoord[ville][3] = 0;
    }
  }

  // parsing du champs EOF
  fgets(line, sizeof(line), f);
  if (!prefixe("EOF", line)) {
    format_error("Warning : no EOF token");
  }

  strcpy(instance->name, name);
  strcpy(instance->type, "TSP");
  instance->dimension = dim;
  strcpy(instance->EDGE_TYPE, "EUC_2D");

  fclose(f);

  return 1;
}