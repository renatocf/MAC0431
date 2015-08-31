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
  int beg, end, t, id;
} sp[TAM];

pthread_mutex_t *mut;

void *slave(void *p)
{
  int i, ii, ini, fim, ticks, id, cnt = 0;
  ini = ((struct ARGS *)p)->beg;
  fim = ((struct ARGS *)p)->end;
  ticks = ((struct ARGS *)p)->t;
  id = ((struct ARGS *)p)->id;
  printf("%d %d %d %d\n", ini, fim, ticks, id);

  for (ii = 0; ii < ticks; ii++) {

    /* black */
    if (id > 0) {
      pthread_mutex_lock(&mut[id-1]);
      blk[ini] = (red[ini-1] + red[ini+1])/2.0; cnt++;
      pthread_mutex_unlock(&mut[id-1]);
    }
    else {
      blk[ini] = (red[ini-1] + red[ini+1])/2.0; cnt++;
    }
    for (i = ini+1; i < fim-1; i++) {
      blk[i] = (red[i-1] + red[i+1])/2.0; cnt++;
    }
    if (id < nprocs-1) {
      pthread_mutex_lock(&mut[id+1]);
      blk[ini] = (red[fim-2] + red[fim])/2.0; cnt++;
      pthread_mutex_unlock(&mut[id+1]);
    }
    else {
      blk[fim-1] = (red[fim-2] + red[fim])/2.0; cnt++;
    }

    /* red */
    if (id > 0) {
      pthread_mutex_lock(&mut[id-1]);
      red[ini] = (blk[ini-1] + blk[ini+1])/2.0; cnt++;
      pthread_mutex_unlock(&mut[id-1]);
    }
    else {
      red[ini] = (blk[ini-1] + blk[ini+1])/2.0; cnt++;
    }
    for (i = ini+1; i < fim-1; i++) {
      red[i] = (blk[i-1] + blk[i+1])/2.0; cnt++;
    }
    if (id < nprocs-1) {
      pthread_mutex_lock(&mut[id+1]);
      red[ini] = (blk[fim-2] + blk[fim])/2.0; cnt++;
      pthread_mutex_unlock(&mut[id+1]);
    }
    else {
      red[fim-1] = (blk[fim-2] + blk[fim])/2.0; cnt++;
    }
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

  mut = malloc(nprocs * sizeof(*mut));

  for (i = 0; i < nprocs; i++) {
    pthread_mutex_init(&mut[i], NULL);
    sizep[i] = (TAM-2)/nprocs;
    if (i < (TAM-2) % nprocs)
      sizep[i]++;
    printf("THR %d :: %d\n", i, sizep[i]);
  }
  sizep[nprocs] = TAM;

  sp[0].beg = 1;
  sp[0].end = sizep[0] + 1;
  sp[0].t   = MAXTIME;
  sp[i].id  = 0;
  pthread_create(&ids[0], NULL, slave, (void *) &sp[0]);

  for (i = 1; i < nprocs; i++) {
    sp[i].beg = sp[i-1].end;
    sp[i].end = sp[i-1].beg + sizep[i];
    sp[i].t   = MAXTIME;
    sp[i].id  = i;
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
