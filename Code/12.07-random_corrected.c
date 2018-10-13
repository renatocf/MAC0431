#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
  int i;
  int nt = 0;

  if (argc > 1) {
    nt = atoi(argv[1]);
  }

  if (nt <= 0) nt = omp_get_num_procs();

# pragma omp parallel num_threads(nt)
  {
    int k = 0;
    int seed = 42;

#   pragma omp for
    for (i = 0; i < 12; i++)
      printf("%d(%d) %d %d\n", i, k++, omp_get_thread_num(), rand_r(&seed));
  }

  return 0;
}
