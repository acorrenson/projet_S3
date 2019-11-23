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

#define TAILLENOM 32 // les noms sont limités à 32 caractères
#define MAXBUF 256   // taille du buffer de lecture des fichiers
#define NIL -1       // indique l'échec dans une fonction

/**
 * @brief Inverse un tableau d'entiers
 *
 */
void reverse(int *, int, int);

/**
 * @brief Trie un segment de tableau d'entiers
 *
 * @param tab Le tableau
 * @param a   Indice de début de segment
 * @param b   Indice de fin de segment
 */
void quick_sort(int *tab, int a, int b);

#endif