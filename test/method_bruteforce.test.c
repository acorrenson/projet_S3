#include <methods/method_bruteforce.h>
#include <testing.h>

void method_bruteforce_run_test() {
  test_t test1;
  init_test(&test1, "testing bruteforce module");

  int p1[3] = {1, 2, 3};
  int p2[3] = {1, 3, 2};
  next_permutation(p1, 3);
  test_ensure(&test1, "next permutation after {1, 2, 3} is {1, 3, 2}",
              test_array_equals(p1, p2, 3));

  int p3[3] = {2, 1, 3};
  next_permutation(p1, 3);
  test_ensure(&test1, "next permutation after {1, 3, 2} is {2, 1, 3}",
              test_array_equals(p1, p3, 3));

  int p4[3] = {2, 3, 1};
  next_permutation(p1, 3);
  test_ensure(&test1, "next permutation after {2, 1, 3} is {2, 3, 1}",
              test_array_equals(p1, p4, 3));

  int p5[3] = {1, 2, 3};
  test_ensure(&test1, "{1, 2, 3} is not the last permutation",
              next_permutation(p5, 3));

  int p6[3] = {3, 2, 1};
  int p7[3] = {3, 2, 1};
  test_ensure(&test1, "{3, 2, 1} is the last permutation",
              !next_permutation(p6, 3));
  test_ensure(&test1, "next permutation of {3, 2, 1} remains {3, 2, 1}",
              test_array_equals(p6, p7, 3));

  end_test(&test1);
}