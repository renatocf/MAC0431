/* programa para difusão de calor */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 10000
#define MAXTIME TAM * 10

double red[TAM] = {100.0, 0.0};
double blk[TAM] = {100.0, 0.0};

pthread_t ids[TAM];
int sizep[TAM];
int nprocs;

struct ARGS {
  int beg, end, t;
} sp[TAM];

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void barrier(int n) {
  static volatile int cnt = 0;
  pthread_mutex_lock(&mut); {
    cnt++;
    if (cnt == n) {
      pthread_cond_broadcast(&cond);
      cnt = 0;
    }
    else {
      pthread_cond_wait(&cond, &mut);
    }
  } pthread_mutex_unlock(&mut);
}

void *slave(void *p)
{
  int i, ii, ini, fim, ticks, cnt = 0;
  ini = ((struct ARGS *)p)->beg;
  fim = ((struct ARGS *)p)->end;
  ticks = ((struct ARGS *)p)->t;
  printf("%d %d %d\n", ini, fim, ticks);

  for (ii = 0; ii < ticks; ii++) {
    for (i = ini; i < fim; i++) {
      blk[i] = (red[i-1] + red[i+1])/2.0;
      cnt++;
    }
    barrier(nprocs);
    for (i = ini; i < fim; i++) {
      red[i] = (blk[i-1] + blk[i+1])/2.0;
      cnt++;
    }
    barrier(nprocs);
  }

  return NULL;
}

int main(int argc, char **argv)
{
  int i;
  void *retval;

  nprocs = argc > 1 ? atoi(argv[1]) : 1;

  if (nprocs < 1) nprocs = 1;
  else if (nprocs >= TAM) nprocs = TAM;

  for (i = 0; i < nprocs; i++) {
    sizep[i] = (TAM-2)/nprocs;
    if (i < (TAM-2) % nprocs)
      sizep[i]++;
    printf("THR %d :: %d\n", i, sizep[i]);
  }
  sizep[nprocs] = TAM;

  sp[0].beg = 1;
  sp[0].end = sizep[0] + 1;
  sp[0].t = MAXTIME;

  pthread_create(&ids[0], NULL, slave, (void *) &sp[0]);
  for (i = 1; i < nprocs; i++) {
    sp[i].beg = sp[i-1].end;
    sp[i].end = sp[i-1].beg + sizep[i];
    sp[i].t = MAXTIME;
    pthread_create(&ids[i], NULL, slave, (void *) &sp[i]);
  }

  for (i = 0; i < nprocs; i++)
    if (pthread_join(ids[i], &retval)) {
      fprintf(stderr, "%d: thread falhou\n", i);
      return EXIT_FAILURE;
    }

  for (i = 0; i < 10; i++) {
    printf(" %7.3f", red[i]);
  }
  printf("\n");

  return EXIT_SUCCESS;
}
