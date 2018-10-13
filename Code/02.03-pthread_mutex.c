#include <stdio.h>
#include <pthread.h>

/* Useful definitions */
typedef enum { false, true } bool;
#define UNUSED(x) (void)(x)
#define EXPRESSION(expr) do { expr; } while (false);

unsigned long int x;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *loop(void *arg) {
  int y, i, j;
  UNUSED(arg);

  for (i = 0; i < 50000; i++) {
    y = 0;
    for (j = 0; j < 10000; j++) {
      y++;
    }
    pthread_mutex_lock(&mut);
    x += y;
    pthread_mutex_unlock(&mut);
  }
  return NULL;
}

int main()
{
  pthread_t id[10];
  int i;
  x = 0;
  for (i = 0; i < 10; i++) pthread_create(&id[i], NULL, loop, NULL);
  for (i = 0; i < 10; i++) pthread_join(id[i], NULL);
  printf("%ld", x);

  return 0;
}
