#ifndef TESTING_H
#define TESTING_H

#include <assert.h>
#include <pretty.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct test_s {
  char name[256];
  unsigned size;
  unsigned passed;
} test_t;

void init_test(test_t *, char *s);
void test_function(test_t *, char *);
void test_ensure(test_t *, char *, bool);
void end_test(test_t *);

bool test_array_equals(int *, int *, int);
bool test_double_equals(double a, double b);
#endif