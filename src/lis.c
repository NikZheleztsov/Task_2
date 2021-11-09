#include "return_status.h"
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

unsigned int
get_num_of_threads ()
{
  unsigned int eax = 11, ebx = 0, ecx = 1, edx = 0;

  asm("cpuid"
      : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
      : "0"(eax), "2"(ecx)
      :);

  return ebx;
}

int
measure_and_solve (int **input_arr, size_t arr_size, FILE *out)
{ // measuring time
  unsigned int num_of_forks = get_num_of_threads () - 1;
  pid_t wait_pid[num_of_forks];
  int status[num_of_forks], pid[num_of_forks];

  int **shared_result = malloc (sizeof (int *) * num_of_forks);
  if (!shared_result)
    {
      fprintf (stderr, "Unable to allocate memory\n");
      return 0;
    }

  for (int i = 0; i < num_of_forks; ++i)
    {
      shared_result[i] = (int *)mmap (NULL, sizeof (int) * (arr_size + 1),
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

      if (shared_result[i] == MAP_FAILED)
        {
          fprintf (stderr, "Unable to map memory\n");
          for (int j = 0; j < i; ++j)
            {
              int result
                  = munmap (shared_result[i], sizeof (int) * (arr_size + 1));
              if (result != 0)
                {
                  fprintf (stderr, "Unable to unmap memory\n");
                  continue;
                }
            }

          return 0;
        }
    }

  // several measurements in order to get the mean time
  // isn't intended to use with parallel algorithm
  // can be used if only, for example, half of the threads is used
  struct timespec start, end;
  unsigned long *sum = (unsigned long *)mmap (
      NULL, sizeof (unsigned long), PROT_READ | PROT_WRITE,
      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  for (int i = 0; i < num_of_forks; ++i)
    {
      pid[i] = fork ();
      if (pid[i] == -1)
        {
          fscanf (stderr, "Failed to create process\n");
          continue;
        }

      if (pid[i] == 0)
        {
          clock_gettime (CLOCK_MONOTONIC, &start);
          int return_value
              = (find_lis (*input_arr, arr_size, shared_result[i]));
          clock_gettime (CLOCK_MONOTONIC, &end);
          *sum += (end.tv_nsec - start.tv_nsec);
          exit (return_value);
        }
    }

  for (int i = 0; i < num_of_forks; ++i)
    wait_pid[i] = waitpid (pid[i], &status[i], 0);

  for (int i = 0; i < num_of_forks; ++i)
      if (wait_pid[i] < 0)
        {
          fprintf (stderr, "waitpid error %d\n", wait_pid[i]);
          return 0;
        }

  int size_of_lis = *shared_result[0];
  fprintf (out, "The solution was found in approximately %ld nanosec\n",
           *sum / num_of_forks);
  fprintf (out, "The length of the longest subsequance is %d\n",
           *shared_result[0]);
  fprintf (out, "The longest increased array: ");
  for (size_t i = 1; i <= shared_result[0][0]; ++i)
    fprintf (out, "%d ", shared_result[0][i]);
  fprintf (out, "\n");

  for (int i = 0; i < num_of_forks; ++i)
    {
      int return_value
          = munmap (shared_result[i], sizeof (int) * (arr_size + 1));
      if (return_value != 0)
        {
          fprintf (stderr, "Error while unmapping\n");
          continue;
        }
    }

  free(shared_result);
  int return_result = munmap (sum, sizeof (unsigned long));
  if (return_result != 0)
    {
      fprintf (stderr, "Error while unmapping\n");
      return 0;
    }

  return size_of_lis;
}
