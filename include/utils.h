/**
 * @file utils.h
 * @author Arthur Correnson (arthur.correnson@univ-tlse3.fr)
 * @brief Collection de fonctions utiles pour le projet
 * @version 0.1
 * @date 2019-11-20
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <pretty.h>
#include <stdio.h>
#include <stdlib.h>

//! les noms sont limités à 32 caractères
#define TAILLENOM 32
//! taille du buffer de lecture des fichiers
#define MAXBUF 256
//! indique l'échec dans une fonction
#define NIL -1

/**
 * @brief Inverse un segment de tableau d'entiers.
 *
 * @param tab   Le tableau
 * @param start Indice de début de segment
 * @param stop  Indice de fin de segment
 */
void reverse(int *tab, int start, int stop);

/**
 * @brief Trie un segment de tableau d'entiers
 *
 * @param tab Le tableau
 * @param a   Indice de début de segment
 * @param b   Indice de fin de segment
 */
void quick_sort(int *tab, int a, int b);

/**
 * @brief Intervertir deux entiers.
 *
 */
void swap(int *, int *);

FILE *read_or_fail(const char *, int);
#endif