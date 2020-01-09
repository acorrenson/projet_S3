/**
 * @file cli.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief Command Line Interface
 * @version 0.1
 * @date 2019-12-24
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef CLI_H
#define CLI_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

//! il y a 13 balises
#define BAL_COUNT 12

// liste des codes pour les balises
//! -f
#define BAL_F 0
//! -t
#define BAL_T 1
//! -v
#define BAL_V 2
//! -o
#define BAL_O 3
//! -ppv
#define BAL_PPV 4
//! -bf
#define BAL_BF 5
//! -bfm
#define BAL_BFM 6
//! -rw
#define BAL_RW 7
//! -2opt
#define BAL_2OPT 8
//! -ga
#define BAL_GA 9
//! -h
#define BAL_H 10
// -nz no zéro
#define BAL_ZERO 11

typedef struct {
  // == Entrées - Sorties ==

  //! Fichier TSP en entrée
  FILE *input_instance;
  //! Fichier CSV en sortie (par défaut stdout)
  FILE *output_csv;
  //! Fichier TOUR en sortie (optionnel)
  FILE *output_tour;
  //! Fichier de log (par défaut stderr)
  FILE *log;

  // == Options pour Algo Génétique ==

  //! Fréquence de mutation (par défaut 0.3)
  double mutation_rate;
  //! Taille de la population (par défaut 20)
  int population_size;
  //! Nombre de générations maximum (par défaut 200)
  int generations;

  // == Informations générales ==

  //! Etat des options (présentes ou non)
  bool state[BAL_COUNT];
} cli_opt_t;

/**
 * @brief Initialise une structure de type cli_opt_t.
 *
 */
void cli_opt__init(cli_opt_t *);

/**
 * @brief Remplis les champs d'une structure cli_opt_t.
 *
 * @param argc Le nombre d'arguments de la ligne de commande.
 * @param argv Les arguments de la ligne de commande.
 * @param opt La structure cli_opt_t.
 */
void cli(int argc, char const *argv[], cli_opt_t *opt);

#endif