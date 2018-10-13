/* Pequeno programa para testar o valgrind com `--tool=memcheck`
 * (padrão) e `--leak-check=full` */

#include <stdio.h>
#include <stdlib.h>

void f(void) {
  int *x = malloc(10 * sizeof (int));
  x[30] = 0;
}

int main(void) {
  f();
  return 0;
}
