#include "lis.h"
#include "interlayer.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

/*
// can't boot this test using github actions
// Unable to read from files out there
void
stress_test ()
{
  char *file_name[] = { "test_5",   "test_1e1",
                        "test_1e2", "test_1e3",
                        "test_1e4", "test_1e5",
                        "test_1e6", "test_1e7",
                        "test_1e8" };

  int right_length[] = { 3, 4, 20, 52, 195, 599, 1784, 4313, 5000 };

  for (int i = 0; i < 9; ++i)
    {
      int *input_arr = NULL;
      size_t arr_size = 0;
      FILE *input_file = fopen (file_name[i], "r");
      if (!input_file)
        {
          fprintf (stderr, "Unable to open file '%s'\n", file_name[i]);
          exit (FAILURE);
        }

      int return_value = fscanf (input_file, "%zu", &arr_size);
      if (return_value != 1)
        {
          fprintf (stderr, "Unable to read file\n");
          exit (FAILURE);
        }

      input_arr = (int *)malloc (sizeof (int) * arr_size);
      if (!input_arr)
        {
          fprintf (stderr, "Unable to allocate memory");
          exit (FAILURE);
        }

      for (size_t i = 0; i < arr_size; ++i)
        fscanf (input_file, "%d", &input_arr[i]);

      return_value = measure_and_solve (&input_arr, arr_size, stdout);
      assert_int_equal (right_length[i], return_value);

      fclose (input_file);
      free (input_arr);
    }
}
*/

// one because he is the only one
void small_test1()
{
    int arr_size = 5;
    int _arr[] = {3, 4, 5, 2, 7};
    int* input_arr = (int*) malloc(sizeof(int) * 5);
    int* result = (int*) malloc(sizeof(int) * 5);

    for (int i = 0; i < 5; ++i)
        input_arr[i] = _arr[i];

    int return_value = find_lis(input_arr, 5, result);
    if (return_value == FAILURE)
        exit(1);

    assert_int_equal (result[0], 4);
    assert_int_equal (result[1], 3);
    assert_int_equal (result[2], 4);
    assert_int_equal (result[3], 5);
    assert_int_equal (result[4], 7);
}


int
main (void)
{
  const UnitTest tests[] = { unit_test (small_test1) };
  return run_group_tests (tests);
}
