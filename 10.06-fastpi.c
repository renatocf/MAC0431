#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 4

/* Fast implementation of pi calculation */

int main() {
  static long num_steps = 4000000;
  double step;

  double pi = 0.0;

  step = 1.0/(double) num_steps;
  omp_set_num_threads(NUM_THREADS);

# pragma omp paralell
  {
    int i, id, nthrds;
    double x, sum;

    id = omp_get_thread_num();
    nthrds = omp_get_num_threads();

    sum = 0.0;
    for (i = id; i < num_steps; i += nthrds) {
      x = (i + 0.5) * step;
      sum += 4.0/(1.0 + x*x);
    }
#   pragma omp atomic
    pi += sum * step;
  }

  printf("pi = %15.12f\n", pi);

  return 0;
}
