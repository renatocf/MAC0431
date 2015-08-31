#include <stdio.h>
#include <math.h>

double f(double x) {
  return 4.0/(1.0 + x*x);
}

int main(void) {
  int i, n;
  double PI25DT = 3.141592653589793238562643;
  double pi, h, sum, x;

  n = 1000000000;
  h = 1.0 / (double) n;
  sum = 0.0;

  for (i = 1; i <= n; i++) {
    x = h * ((double) i - 0.5);
    sum += f(x);
  }

  pi = h * sum;

  printf("pi is approximately %.16f, Error is %.16f\n",
          pi, fabs(pi - PI25DT));

  return 0;
}
