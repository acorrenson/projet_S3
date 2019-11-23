/**
 * @file testing.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief Collection de fonctions pour faciliter l'écriture des tests unitaires
 * @version 0.1
 * @date 2019-11-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef TESTING_H
#define TESTING_H

#include <assert.h>
#include <pretty.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @brief Structure pour représenter un block de test.
 * 
 */
typedef struct test_s {
  char name[256];   /**< nom du block. */
  unsigned size;    /**< taille du block. */
  unsigned passed;  /**< nombre de tests réussis. */
} test_t;

/**
 * @brief Initialise un block de test.
 * 
 * @param name Nom du block.
 */
void init_test(test_t *, char *name);

/**
 * @brief Affiche le résultat d'un test et met à jour l'état du block de test en conséquence.
 * 
 * @param name    Description du test effectué.
 * @param result  Résultat du test (doit être "true" pour être validé).
 */
void test_ensure(test_t *, char *name, bool result);

/**
 * @brief Affiche le bilan après execution d'un block de test.
 * 
 */
void end_test(test_t *);

/**
 * @brief Utilitaire de comparison de tableau.
 * 
 */
bool test_array_equals(int *, int *, int);

/**
 * @brief Utilitaire de comparison de double (à e-3 près).
 * 
 */
bool test_double_equals(double, double);

#endif