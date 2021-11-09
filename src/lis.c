#include "interlayer.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

size_t
binary_search_for_upper_bound (int *arr, int *subarr, size_t ind,
                               size_t size_subarray)
{
  // Binary search for the smalles element, which is as big
  // as arr[ind]
  size_t left = 0, right = size_subarray - 1, mid;
  while (right > left)
    {
      mid = (right + left) / 2;
      if (arr[subarr[mid]] < arr[ind])
        left = mid + 1;
      else
        right = mid;
    }

  return right;
}

int
find_lis (int *arr, size_t size, int *result)
{
  // Only indexes in these arrays
  // temp is the smallest element of the subsequence
  // i == length of the subsequence
  int *temp = (int *)malloc (sizeof (int) * (size));
  int *parent = (int *)malloc (sizeof (int) * (size));

  if (!temp || !parent)
    {
      fprintf (stderr, "Unable to allocate memory\n");
      free (temp);
      free (parent);
      return FAILURE;
    }

  for (size_t i = 0; i < size; ++i)
    parent[i] = -1;

  size_t current_pos = 1;
  temp[0] = 0;

  for (size_t i = 1; i < size; ++i)
    if (arr[i] > arr[temp[current_pos - 1]])
      {
        parent[i] = temp[current_pos - 1];
        temp[current_pos++] = i;
      }
    else
      {
        size_t location_to_repl
            = binary_search_for_upper_bound (arr, temp, i, current_pos);

        temp[location_to_repl] = i;
        if (location_to_repl != 0)
          parent[i] = temp[location_to_repl - 1];
      }

  int curr_parent = temp[current_pos - 1];

  result[0] = current_pos;
  for (size_t i = current_pos; i > 0 && curr_parent != -1; --i)
    {
      result[i] = arr[curr_parent];
      curr_parent = parent[curr_parent];
    }

  free(parent);
  free(temp);
  return SUCCESS;
}
