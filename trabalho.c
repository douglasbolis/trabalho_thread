/**
 * trabalho.c
 *
 * Copyright 2017 Douglas <douglasbolislima@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/**
 * Características da matriz
 */
// #define TAMLINHA 20000
// #define TAMCOLUNA 20000
// #define MAXRANDOM 29999
#define TAMLINHA 5
#define TAMCOLUNA 5
#define MAXRANDOM 100

unsigned int countPrime = 0;

/**
 * Assinatura dos métodos.
 */
int isPrime( int );
int** createMatrix( int, int );
void fillMatrix( int**, int, int );
void printMatrix( int**, int, int );
void serialSearch( int**, int, int );
void freeMatrix( int**, int );

/**
 * Método principal.
 */
int main( int argc, char **argv ) {
    int **matrix;
    int lengthLine = TAMLINHA;
    int lengthColumn = TAMCOLUNA;

    /** criando a matriz */
    matrix = createMatrix( lengthLine, lengthColumn );

    /** adicionando números aleatórios */
    fillMatrix( matrix, lengthLine, lengthColumn );

    /** Imprimindo matriz */
    printMatrix( matrix, lengthLine, lengthColumn );

    /** BUSCA SERIAL */
    serialSearch( matrix, lengthLine, lengthColumn);
    printf( "\nBUSCA SERIAL\nNúmeros primos encontrados na matriz: %u\n", countPrime );
    /** FIM BUSCA SERIAL */

    /** Libera espaço alocado para a matriz */
    freeMatrix( matrix, lengthLine );
    return 0;
}

int** createMatrix( int line, int column ) {
  int i;
  int **matrix = malloc( line * sizeof( unsigned long ) );

  for( i = 0; i < line; i++ ) {
    matrix[ i ] = malloc( column * sizeof( int ) );
  }

  return matrix;
}

void fillMatrix( int **matrix, int line, int column ) {
  int i, j;

  srand( ( unsigned )time( NULL ) );
  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
       /** ( MAXRANDOM + 1 ) para que o número máximo definido também possa ser gerado. */
      matrix[ i ][ j ] = rand() % ( MAXRANDOM + 1 );
    }
  }
}

void serialSearch( int **matrix, int line, int column ) {
  int i, j;

  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
      countPrime += isPrime( matrix[ i ][ j ] );
    }
  }
}

int isPrime( int n ) {
  double limit;
  int i = 3, control = 1;

  if ( n == 2 ) {
    return 1;
  } else if ( n < 2 || n % 2 == 0 ) {
    return 0;
  } else {
    limit = sqrt( n ) + 1;
    while ( i < limit && control ) {
      if ( n % i == 0 ) {
        control = 0;
      }
      i += 2;
    }
    return control;
  }
}

void printMatrix( int **matrix, int line, int column ) {
  int i, j;

  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
      printf( " %5d ", matrix[ i ][ j ] );
    }
    printf( "\n" );
  }
}

void freeMatrix( int **matrix, int line ) {
  int i;
  for ( i = 0; i < line; i++ ) {
    free( matrix[ i ] );
  }
  free( matrix );
}
