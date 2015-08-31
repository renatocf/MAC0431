/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* POSIX thread headers */
#include <pthread.h>

typedef void (*FPTR)(void *);
typedef unsigned long int ugly;

#define NIT 500000000UL
#define MAXTHR 3200

double pi[MAXTHR];
unsigned int NT = 1;

double f(double x) {
  return 4.0 / (1.0 + x*x);
}

void *t(void *nn) {
  ugly i;
  ugly n = (ugly) nn;

  double h = 1.0/NIT;

  double mypi;
  double sum = 0.0;

  for (i = n; i < NIT; i+= NT) {
    mypi = h * ((double) i - 0.5);
    sum += f(mypi);
  }
  pi[n] *= sum;

  return (void *) i;
}

int main (int argc, char **argv) {
  pthread_t id[32];
  void *retval;
  ugly i;
  double res = 0;

  if (argc > 1) {
    NT = atoi(argv[1]);
    if (NT < 1) NT = 1;
    if (NT > MAXTHR) NT = MAXTHR;
  }

  for (i = 0; i < NT; i++)
    pthread_create(&id[i], NULL, t, (void *)i); /* ugly trick */

  for (i = 0; i < NT; i++) {
    if (pthread_join(id[i], &retval) != 0)
      fprintf(stderr, "Error on thread %ld", i);
  }

  for (i = 0; i < NT; i++) res += pi[i];
  printf("pi = %f\n", res);

  return EXIT_SUCCESS;
}
