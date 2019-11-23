#include <testing.h>

void init_test(test_t *test, char *name) {
  strcpy(test->name, name);
  test->passed = 0;
  test->size = 0;
  printf("BEGIN test [" COLOR_Y "%s" COLOR_N "]\n", name);
}

void test_ensure(test_t *test, char *description, bool result) {
  test->size++;
  printf("  - testing if " COLOR_Y "%s" COLOR_N "\n", description);
  if (result) {
    printf("  -> " COLOR_G "OK\n" COLOR_N);
    test->passed++;
  } else {
    printf("  -> " COLOR_R "NOT OK\n" COLOR_N);
  }
}

void end_test(test_t *test) {
  printf("END test [" COLOR_Y "%s" COLOR_N "] ", test->name);
  printf("[" COLOR_G "%d" COLOR_N "/" COLOR_G "%d" COLOR_N "] tests passed\n\n",
         test->passed, test->size);
}

bool array_equals(int *a, int *b, int d) {
  return memcmp(a, b, d * sizeof(int)) == 0;
}