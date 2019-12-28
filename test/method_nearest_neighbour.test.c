#include <methods/method_nearest_neighbour.h>
#include <testing.h>
#include <utils.h>

void method_nn_run_test() {
  test_t test;
  init_test(&test, "testing nearest-neighbour module");
  instance_t inst;
  instance__init(&inst, true);
  instance__read_from_file(&inst, read_or_fail("./test.tsp", 0));

  instance__mark(&inst, 0);
  bool check1 = inst.tabCoord[0][2] == 1;
  test_ensure(&test, "the 'mark' function can set node 0 as marked", check1);

  bool check2 = instance__marked(&inst, 0);
  test_ensure(&test, "if node 0 is marked, 'marked' returns true", check2);

  tour_t res;
  nearest_neighbour(&inst, &res);
  bool check3 = true;
  for (int i = 0; i < inst.dimension; i++)
    check3 = check3 && instance__marked(&inst, i);
  test_ensure(&test,
              "all nodes are marked after applying nearest_neighbour method",
              check3);

  int sol[3] = {0, 1, 2};
  bool check4 = test_array_equals(res.tour, sol, 3);
  test_ensure(&test,
              "applying nearest_neighbour on test.tsp gives result {0, 1, 2}",
              check4);

  bool check5 = test_double_equals(res.length, 5.236);
  test_ensure(&test, "the length of the tour {0, 1, 2} is set to 5.236",
              check5);

  end_test(&test);
}