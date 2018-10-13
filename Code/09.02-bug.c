#include <stdio.h>

int main()
{
  int i, j;

  j = 42;
  i = 1;
  while (j > 1) {
    j = j/i;
  }
  return 0;
}
