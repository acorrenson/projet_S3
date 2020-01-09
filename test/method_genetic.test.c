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

  test_ensure(&test, "[1, 2, 3, 4] & [1, 4, 3, 2] share 1 fragment", size == 1);

  test_ensure(&test,
              "[1, 2, 3, 4] & [1, 4, 3, 2] share the fragments [1, 2, 3, 4]",
              memcmp(arr3, shared_fragments[0], 4 * sizeof(int)) == 0);

  end_test(&test);
}