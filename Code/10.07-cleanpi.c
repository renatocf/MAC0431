#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 4

/* Fast implementation of pi calculation */

int main() {
  int i;
  static long num_steps = 4000000;

  double step = 1.0/(double) num_steps;
  double pi = 0.0;
  double x, sum = 0.0;

# pragma omp parallel for private(x) reduction(+:sum)
  for (i = 0; i < num_steps; i++) {
    x = (i + 0.5) * step;
    sum += 4.0/(1.0 + x*x);
  }

  pi = sum * step;

  printf("pi = %15.12f\n", pi);

  return 0;
}
