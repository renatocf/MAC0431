#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv) {
  int i;
  int j;
  int n = 200000;
  int p_num;
  int prime;
  int t_num = 0;
  int total = 0;

  printf("\n");
  printf("PRIME_SUM\n");
  printf(" C/OpenMP version\n");
  printf(" Sum the primes from 1 to %d\n", n);

  p_num = omp_get_num_procs();

  if (argc > 1)
    t_num = atoi(argv[1]);

  if (t_num < 1)
    t_num = p_num;

  omp_set_num_threads(t_num);

  printf(" Number of processors available is %d\n", p_num);
  printf(" Number of threads requested is    %d\n", t_num);

  /* 'i' é private por padrão */
  /* 'n' é shared por padrão*/
# pragma omp parallel for private (i, prime, j) shared (n) reduction (+:total) schedule (guided, 1000) /* 1000 */
  for (i = n; i >= 2; i--) {
    prime = 1;

    for (j = 2; j < i; j++)
      if (i % j == 0) {
        prime = 0;
        break;
      }

    if (prime)
      total = total + i;
  }

  printf("\n");
  printf("PRIME_SUM(2:%d) = %d\n", n, total);

  return EXIT_SUCCESS;
}
