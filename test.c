#include <stdio.h>
#include <stdlib.h>

int main() {
  int test = 0;

  printf( "test: %d\n", test );

  printf( "test: %d\n", ++test );

  printf( "test: %d\n", test );

  printf( "test: %d\n", test++ );

  printf( "test: %d\n", test );

  return 0;
}
