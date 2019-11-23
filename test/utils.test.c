#include "testing.h"
#include <utils.h>

void utils_run_test() {
  test_t test1;
  init_test(&test1, "testing testing");
  test_ensure(&test1, "true is true", true);
  test_ensure(&test1, "true is not false", true == !false);
  test_ensure(&test1, "true and false are different", true != false);
  test_ensure(&test1, "1 equals 2", 1 == 2);
  test_ensure(&test1, "1.1 equals 1.1", test_double_equals(1.1, 1.1));
  test_ensure(&test1, "1.1111 equals 1.1112",
              test_double_equals(1.1111, 1.1112));
  test_ensure(&test1, "1.111 differs from 1.112",
              !test_double_equals(1.111, 1.112));
  end_test(&test1);

  init_test(&test1, "testing utils functions");
  int t1[3] = {0, 0, 0};
  int t2[3] = {0, 0, 0};
  reverse(t1, 0, 2);
  test_ensure(&test1, "reversing {0, 0, 0} gives {0, 0, 0}",
              test_array_equals(t1, t2, 3));
  int t3[3] = {1, 2, 3};
  int t4[3] = {3, 2, 1};
  reverse(t3, 0, 2);
  test_ensure(&test1, "reversing {1, 2, 3} gives {3, 2, 1}",
              test_array_equals(t3, t4, 3));

  int t5[5] = {1, 2, 3, 4, 5};
  int t6[5] = {1, 2, 3, 4, 5};
  quick_sort(t5, 0, 4);
  test_ensure(&test1, "sorting {1, 2, 3, 4, 5} gives {1, 2, 3, 4, 5}",
              test_array_equals(t5, t6, 5));

  int t7[5] = {5, 4, 3, 2, 1};
  int t8[5] = {1, 2, 3, 4, 5};
  quick_sort(t7, 0, 4);
  test_ensure(&test1, "sorting {5, 4, 3, 2, 1} gives {1, 2, 3, 4, 5}",
              test_array_equals(t7, t8, 5));

  int t9[5] = {-1, -2, 3, 2, 0};
  int t10[5] = {-2, -1, 0, 2, 3};
  quick_sort(t9, 0, 4);
  test_ensure(&test1, "sorting {-1, -2, 3, 2, 0} gives {-2, -1, 0, 2, 3}",
              test_array_equals(t9, t10, 5));

  int t11[1] = {1};
  int t12[1] = {1};
  quick_sort(t11, 0, 0);
  test_ensure(&test1, "sorting {1} gives {1}", test_array_equals(t11, t12, 1));

  end_test(&test1);
}
