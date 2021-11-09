#include "lis.h"
#include "return_status.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>

C_TEST(stress_test)
{
  char file_name[]
      = { "test_5",   "test_1e1", "test_1e2", "test_1e3", "test_1e4",
          "test_1e5", "test_1e6", "test_1e7", "test_1e8" };

  int right_length[] = {3, 4, 20, 52, 195, 599, 1784, 4313, 5000};

  for (int i = 0; i < 9; ++i)
    {
      int *input_arr = NULL;
      size_t arr_size = 0;
      FILE *input_file = fopen (&file_name[i], "r");
      if (!input_file)
        {
          fprintf (stderr, "Unable to open file '%s'\n", &file_name[i]);
          return FAILURE;
        }

      int return_value = fscanf (input_file, "%zu", &arr_size);
      if (return_value != 1)
        {
          fprintf (stderr, "Unable to read file\n");
          return FAILURE;
        }

      input_arr = (int *)malloc (sizeof (int) * arr_size);
      if (!input_arr)
        {
          fprintf (stderr, "Unable to allocate memory");
          return FAILURE;
        }

      for (size_t i = 0; i < arr_size; ++i)
        fscanf (input_file, "%d", &input_arr[i]);

      return_value = measure_and_solve (&input_arr, arr_size, stdout);
      EXPECT_EQ(right_length[i] == return_value);

      fclose (input_file);
      free(input_arr);
    }
}
