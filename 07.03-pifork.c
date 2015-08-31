/* System libraries */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>

/* Standard libraries */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NIT 500000000

double f(double x) {
  return 4.0 / (1.0 + x*x);
}

typedef double *SHARED;

SHARED x;

int main(int argc, char **argv) {
  unsigned long int i, ii;
  int smid;
  double h = 1.0/NIT;
  double mypi = 0;
  unsigned long int nprocs;
  double sum = 0.0;

  nprocs = 1;
  if (argc > 1)
    nprocs = atoi(argv[1]);
  if (nprocs < 1) nprocs = 1;

  smid = shmget (IPC_PRIVATE, sizeof(double) * nprocs, IPC_CREAT | 0600);
  if (smid == -1) {
    perror("Erro de alicação");
    return EXIT_FAILURE;
  }

  x = (SHARED) shmat(smid, 0, 0);

  for (ii = 1; ii < nprocs; ii++) {
    if ((fork()) == 0) {
      printf("Iniciando filho %lu\n", ii);
      for (i = ii; i < NIT; i += nprocs) {
        mypi = h * ((double) i + 0.5);
        sum += f(mypi);
      }
      x[ii] = mypi;
      return EXIT_SUCCESS;
    }
  }

  for (i = 0; i < NIT; i+= nprocs) {
    mypi = h * ((double)i + 0.5);
    sum += f(mypi);
  }
  mypi = h * sum;

  /* espera cada um dos filhos terminar */
  for (i = 1; i < nprocs; i++) {
    wait(NULL);
  }

  for (i = 1; i < nprocs; i++) {
    mypi += x[i];
  }

  printf("\n\npi = %.8f\n", mypi);
  /* shm */

  return EXIT_SUCCESS;
}
