#ifndef INTERLAYER_H
#define INTERLAYER_H
#include <stdio.h>

#define SUCCESS 0
#define FAILURE 1

int measure_and_solve (int **input_arr, size_t arr_size, FILE *out);

#endif
