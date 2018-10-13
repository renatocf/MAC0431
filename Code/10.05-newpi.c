#include <stdio.h>
#include <omp.h>
#define PAD 4096
#define NUM_THREADS 4

/* New implementation of pi calculation (with page size) */

int main() {
  static long num_steps = 4000000;
  double step;

  int i, nthreads;

  /* Assumes page size PAD = 4096. Is a machine-dependend info */
  double pi = 0.0, sum[NUM_THREADS][PAD];

  step = 1.0/(double) num_steps;
  omp_set_num_threads(NUM_THREADS);

# pragma omp paralell
  {
    int i, id, nthrds;
    double x;

    id = omp_get_thread_num();
    nthrds = omp_get_num_threads();

    if (id == 0) nthreads = nthrds;
    sum[id][0] = 0.0;
    for (i = id; i < num_steps; i+= nthrds) {
      x = (i + 0.5) * step;
      sum[id][0] += 4.0/(1.0 + x*x);
    }
    sum[id][0] *= step;
  }

  pi = 0;
  for (i = 0; i < nthreads; i++) {
    pi += sum[i][0];
  }

  printf("pi = %15.12f\n", pi);

  return 0;
}
