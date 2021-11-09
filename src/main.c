#include "return_status.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef SERIAL
#include "lis.h"
#endif

void
help (FILE *out, char *prog_name)
{
  fprintf (out, "Usage: %s [OPTIONS]...\n", prog_name);
  fprintf (out, "\nAvailable options:\n");
  fprintf (out, "  -h           shows this help info\n");
  fprintf (out, "  -i [FILE]    gets data from the FILE\n");
  fprintf (out, "  -o [FILE]    output data to the FILE\n\n");
}


int
main (int argc, char **argv)
{
  char *input_file_name = NULL, *output_file_name = NULL;

  // parsing command line arguments
  int opt;
  opterr = 0;

  while ((opt = getopt (argc, argv, "hi:o:")) != -1)
    switch (opt)
      {
      case 'i':
        input_file_name = optarg;
        break;

      case 'o':
        output_file_name = optarg;
        break;

      case 'h':
        help (stdout, argv[0]);
        return (SUCCESS);

      case '?':
        if (optopt == 'i' || optopt == 'o')
          fprintf (stderr, "Option -%c requires an argument\n", optopt);
        else
          fprintf (stderr, "Unknown option '-%c'\n", optopt);
        return FAILURE;

      default:
        return FAILURE;
      }

  if (output_file_name && !input_file_name)
    {
      fprintf (stderr, "Unable to enter interactive mode\n");
      fprintf (stderr, "Please, provide input file or delete output one\n");
      return EXIT_FAILURE;
    }

  // output
  FILE *out = NULL;
  if (output_file_name)
    {
      out = fopen (optarg, "w");
      if (!out)
        {
          fprintf (stderr, "Unable to open file '%s'", optarg);
          return EXIT_FAILURE;
        }
    }
  else
    out = stdout;

  size_t arr_size = 0;
  int *input_arr = NULL;
  int function_status = FAILURE;
  int return_value;

  // Interactive mode
  if (input_file_name == NULL)
    {
      int return_value = 0, num_of_tries = 5;
      printf ("No file name was provided. Entering iteractive mode...\n");
      while (return_value != 1 && !input_arr)
        {
          if (num_of_tries == 0)
            return FAILURE;

          printf ("Please, enter the size of the subsequent array: ");
          return_value = scanf ("%zu", &arr_size);

          if (return_value != 1)
            {
              fprintf (stderr, "Error while reading input\n");
              printf ("You have %d more tries\n", num_of_tries--);
              continue;
            }

          input_arr = (int *)malloc (sizeof (int) * arr_size);
          if (!input_arr)
            {
              fprintf (stderr, "Unable to allocate memory");
              printf ("You have %d more tries\n", num_of_tries--);
            }
        }

      printf ("Please, enter array values: ");
      for (size_t i = 0; i < arr_size; ++i)
        scanf ("%d", &input_arr[i]);
    }
  else
    {
      FILE *input_file = fopen (input_file_name, "r");
      if (!input_file)
        {
          fprintf (stderr, "Unable to open file '%s'\n", input_file_name);
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

      fclose (input_file);
    }

  return_value = measure_and_solve (&input_arr, arr_size, out);
  if (return_value == 0)
    return FAILURE;

  free (input_arr);

  if (output_file_name)
    fclose (out);

  return 0;
}
