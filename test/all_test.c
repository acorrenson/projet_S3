#include "all_test.h"

int main(int argc, char const *argv[]) {
  utils_run_test();
  tsplib_run_test();
  method_bruteforce_run_test();
  method_nn_run_test();
  method_genetic_run_test();
}