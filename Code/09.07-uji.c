/* Pequeno programa para testar o valgrind com `--tool=cachegrind` */

#include <stdio.h>

#define TAM 10000
double a[TAM][TAM] = {0.,};

int main() {
  int i, j;
  for (j = 0; j < TAM; j++) {
    for (i = 0; i < TAM; i++) {
      a[i][j] = 1;
    }
  }
}
