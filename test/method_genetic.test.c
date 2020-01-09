#include <assert.h>
#include <methods/method_genetic.h>
#include <testing.h>

void method_genetic_run_test() {
  test_t test;
  init_test(&test, "testing genetic module");

  tour_t tour_test_1;
  tour_t tour_test_2;
  int arr1[] = {1, 2, 3};
  int arr2[] = {2, 3, 1};
  int arr3[] = {1, 2, 3, 4};
  int arr4[] = {1, 2, 4, 3};
  tour__from_array(&tour_test_1, arr1, 3);
  tour__from_array(&tour_test_2, arr2, 3);

  int **shared_fragments;
  int *sizes;
  int size;

  size = get_shared_fragments(&tour_test_1, &tour_test_2, &shared_fragments,
                              &sizes);

  test_ensure(&test, "There is 1 shared fragment btw [1, 2, 3] and [2, 3, 1]",
              size == 1);

  test_ensure(&test, "[1, 2, 3] & [2, 3, 1] share the fragment [1, 2, 3]",
              memcmp(arr1, shared_fragments[0], 3 * sizeof(int)) == 0);

  int arr5[] = {1, 2};
  int arr6[] = {3, 4};

  free(tour_test_1.tour);
  free(tour_test_2.tour);
  free(sizes);
  for (int i = 0; i < 3; i++)
    free(shared_fragments[i]);
  tour__set_dimension(&tour_test_1, 4);
  tour__set_dimension(&tour_test_2, 4);
  tour__from_array(&tour_test_1, arr3, 4);
  tour__from_array(&tour_test_2, arr4, 4);

  size = get_shared_fragments(&tour_test_1, &tour_test_2, &shared_fragments,
                              &sizes);

  test_ensure(&test, "[1, 2, 3, 4] & [1, 2, 4, 3] share 2 fragments",
              size == 2);

  test_ensure(&test, "[1, 2, 3, 4] & [1, 2, 4, 3] share the fragments [1, 2]",
              memcmp(arr5, shared_fragments[0], 2 * sizeof(int)) == 0);

  test_ensure(&test, "[1, 2, 3, 4] & [1, 2, 4, 3] share the fragments [3, 4]",
              memcmp(arr6, shared_fragments[1], 2 * sizeof(int)) == 0);

  int arr7[] = {1, 4, 3, 2};

  free(tour_test_1.tour);
  free(tour_test_2.tour);
  free(sizes);
  for (int i = 0; i < 4; i++)
    free(shared_fragments[i]);
  tour__set_dimension(&tour_test_1, 4);
  tour__set_dimension(&tour_test_2, 4);
  tour__from_array(&tour_test_1, arr3, 4);
  tour__from_array(&tour_test_2, arr7, 4);

  size = get_shared_fragments(&tour_test_1, &tour_test_2, &shared_fragments,
                              &sizes);
  printf("size : %d\n", size);
  test_ensure(&test, "[1, 2, 3, 4] & [1, 4, 3, 2] share 1 fragment", size == 1);

  test_ensure(&test,
              "[1, 2, 3, 4] & [1, 4, 3, 2] share the fragments [1, 2, 3, 4]",
              memcmp(arr3, shared_fragments[0], 4 * sizeof(int)) == 0);

  // tour_t t1;
  // tour_t t2;
  // tour__init(&t1);
  // tour__init(&t2);
  // tour__set_dimension(&t1, 3);
  // tour__set_dimension(&t2, 3);

  // int a1[3] = {1, 2, 3};
  // int a2[3] = {2, 1, 3};

  // for (int i = 0; i < 3; i++) {
  //   t1.tour[i] = a1[i];
  //   t2.tour[i] = a2[i];
  // }

  // int edges1[3][2] = {{1, 2}, {2, 3}, {1, 3}};
  // int edges2[3][2] = {{1, 2}, {1, 3}, {2, 3}};

  // int **cedges1;
  // int **cedges2;

  // tour__get_edges(&t1, &cedges1);
  // tour__get_edges(&t2, &cedges2);

  // // for (int i = 0; i < 3; i++) {
  // //   printf("edge : (%d %d) =? (%d %d) : %d\n", cedges2[i][0],
  // cedges2[i][1],
  // //          edges2[i][0], edges2[i][1], cedges2[i][0] == edges2[i][0],
  // //          cedges2[i][1] == edges2[i][1]);
  // // }

  // bool check1 = true;
  // bool check2 = true;

  // for (int i = 0; i < 3; i++) {
  //   check1 = check1 &&
  //            (edges1[i][0] == cedges1[i][0] && edges1[i][1] ==
  //            cedges1[i][1]);
  //   check2 = check2 &&
  //            (edges2[i][0] == cedges2[i][0] && edges2[i][1] ==
  //            cedges2[i][1]);
  // }

  // test_ensure(&test, "edges of tour [1, 2, 3] are {{1, 2}, {2, 3}, {1,
  // 3}}",
  //             check1);
  // test_ensure(&test, "edges of tour [2, 1, 3] are {{1, 2}, {1, 3}, {2,
  // 3}}",
  //             check2);

  // bool check3 = true;
  // int **frags;
  // int *sizes;
  // int size = get_shared_fragments(&t1, &t2, &frags, &sizes);

  // test_ensure(&test, "There is 1 shared fragment bt [2, 1, 3] and [1, 2,
  // 3]",
  //             size == 1);

  // test_ensure(&test,
  //             "The Shared fragment bt [2, 1, 3] and [1, 2, 3] has size
  //             3", sizes[0] == 3);

  // for (int i = 0; i < 1; i++) {
  //   check3 = check3 && (frags[0][i] == i + 1);
  // }

  // test_ensure(&test, "shared fragments bt [2, 1, 3] and [1, 2, 3] is {1,
  // 2, 3}",
  //             check3);

  // free(sizes);
  // free(frags);

  // test_ensure(&test,
  //             "there is only 1 shared fragment bt [1, 2, 3] and [1, 2,
  //             3]", get_shared_fragments(&t1, &t1, &frags, &sizes) == 1);

  // check3 = true;
  // for (int i = 0; i < 3; i++) {
  //   check3 = check3 && (frags[0][i] == i + 1);
  //   printf("%d ", frags[0][i]);
  // }

  // test_ensure(
  //     &test,
  //     "the only fragments shared bt [1, 2, 3] and [1, 2, 3] is [1, 2,
  //     3]", check3);

  // tour_t t3;
  // tour_t t4;

  // tour__init(&t3);
  // tour__init(&t4);
  // tour__set_dimension(&t3, 10);
  // tour__set_dimension(&t4, 10);

  // int a3[] = {5, 3, 9, 1, 2, 8, 10, 6, 7, 4};
  // int a4[] = {1, 2, 5, 3, 9, 4, 8, 6, 10, 7};

  // for (int i = 0; i < 10; i++) {
  //   t3.tour[i] = a3[i];
  //   t4.tour[i] = a4[i];
  // }

  // int **edgesA;
  // int **edgesB;
  // tour__get_edges(&t3, &edgesA);
  // tour__get_edges(&t4, &edgesB);

  // int cedgesA[10][2] = {{3, 5},  {3, 9},  {1, 9}, {1, 2}, {2, 8},
  //                       {8, 10}, {6, 10}, {6, 7}, {4, 7}, {4, 5}};

  // int cedgesB[10][2] = {{1, 2}, {2, 5}, {3, 5},  {3, 9},  {4, 9},
  //                       {4, 8}, {6, 8}, {6, 10}, {7, 10}, {1, 7}};

  // bool check = true;
  // for (int i = 0; i < 10; i++) {
  //   check = check &&
  //           (cedgesA[i][0] == edgesA[i][0] && cedgesA[i][1] ==
  //           edgesA[i][1]);
  // }

  // test_ensure(&test,
  //             "edges of [5, 3, 9, 1, 2, 8, 10, 6, 7, 4] are "
  //             "(3 5) (3 9) (1 9) (1 2) (2 8) (8 10) (6 10) (6 7) (4 7) (4
  //             5)", check);

  // check = true;
  // for (int i = 0; i < 10; i++) {
  //   check = check &&
  //           (cedgesB[i][0] == edgesB[i][0] && cedgesB[i][1] ==
  //           edgesB[i][1]);
  // }

  // test_ensure(&test,
  //             "edges of [1, 2, 5, 3, 9, 4, 8, 6, 10, 7] are "
  //             "(1 2) (2 5) (3 5) (3 9) (4 9) (4 8) (6 8) (6 10) (7 10) (1
  //             7)", check);

  // instance_t inst;
  // tour_t t5;
  // int *sizes2;
  // int **frags2;
  // int size2 = get_shared_fragments(&t3, &t4, &frags2, &sizes2);
  // instance__init(&inst, false);
  // instance__read_from_file(&inst, read_or_fail("./att10.tsp", 0));
  // instance__compute_distances(&inst);
  // int tour[10] = {5, 3, 9, 1, 2, 4, 10, 6, 7, 8};
  // cross_dpx(&inst, &t3, &t4, &t5);

  // check = true;
  // for (int i = 1; i < 10; i++) {
  //   check = check && (tour__has_node(&t5, i));
  // }

  // test_ensure(&test,
  //             "The result of cross_dpx on "
  //             "{5, 3, 9, 1, 2, 8, 10, 6, 7, 4} and "
  //             "{1, 2, 5, 3, 9,4, 8, 6, 10, 7} is consistent",
  //             check);

  // check = true;
  // for (int i = 0; i < 10; i++) {
  //   check = check && (t5.tour[i] == tour[i]);
  // }

  // test_ensure(&test,
  //             "The result of cross_dpx on "
  //             "{5, 3, 9, 1, 2, 8, 10, 6, 7, 4} and "
  //             "{1, 2, 5, 3, 9,4, 8, 6, 10, 7} is "
  //             "{5, 3, 9, 1, 2, 4, 10, 6, 7, 8}",
  //             check);

  // tour_t res;
  // cli_opt_t opt;

  // opt.log = stderr;
  // opt.state[BAL_V] = true;
  // genetic(&inst, &res, &opt);

  // test_ensure(&test,
  //             "The result of the genetic method and the instance have the
  //             " "same dimension", res.dimension == inst.dimension);

  // check = true;
  // for (int i = 0; i < inst.dimension; i++) {
  //   check = check && (tour__has_node(&res, instance__node_at(&inst, i)));
  //   printf("%d ", res.tour[i]);
  // }
  // printf("\n");

  // test_ensure(&test, "The result of the genetic method is consistent",
  // check);

  // tour_t test_1;
  // tour_t test_2;
  // tour__set_dimension(&test_1, 4);
  // tour__set_dimension(&test_2, 4);
  // int t_1[4] = {1, 2, 3, 4};
  // int t_2[4] = {1, 2, 4, 3};

  // for (int i = 0; i < 4; i++) {
  //   test_1.tour[i] = t_1[i];
  //   test_2.tour[i] = t_2[i];
  // }

  // int **frags_2;
  // int *sizes_2;
  // int size_2;
  // size = get_shared_fragments(&test_1, &test_2, &frags_2, &sizes_2);
  // for (int i = 0; i < size_2; i++) {
  //   for (int j = 0; j < sizes_2[i]; j++) {
  //     printf("%d ", frags_2[i][j]);
  //   }
  //   printf("\n");
  // }

  end_test(&test);
}