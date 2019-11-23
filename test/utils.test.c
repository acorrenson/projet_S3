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
  reverse(t1, 3);
  test_ensure(&test1, "reversing {0, 0, 0} gives {0, 0, 0}",
              test_array_equals(t1, t2, 3));
  int t3[3] = {1, 2, 3};
  int t4[3] = {3, 2, 1};
  reverse(t3, 3);
  test_ensure(&test1, "reversing {1, 2, 3} gives {3, 2, 1}",
              test_array_equals(t3, t4, 3));
  end_test(&test1);
}
