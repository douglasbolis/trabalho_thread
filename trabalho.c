/**
 * trabalho.c
 *
 * Copyright 2017 Douglas <douglasbolislima@gmail.com>
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/**
 * Características da matriz
 */
#define TAMLINHA 20000
#define TAMCOLUNA 20000
#define MAXRANDOM 29999

unsigned int countPrime = 0;

/**
 * Assinatura dos métodos
 */
int** createMatriz( int, int );
int isPrime( int );
void fillMatriz( int**, int, int );
void printMatriz( int**, int, int );
void serialSearch( int**, int, int );
void freeMatriz( int**, int );

/**
 * Método principal
 */
int main( int argc, char **argv ) {
    int **matriz;
    int lengthLine = TAMLINHA;
    int lengthColumn = TAMCOLUNA;

    /** criando a matriz */
    matriz = createMatriz( lengthLine, lengthColumn );

    /** adicionando números aleatórios */
    fillMatriz( matriz, lengthLine, lengthColumn );

    /** BUSCA SERIAL */
    serialSearch( matriz, lengthLine, lengthColumn);
    printf( "\nBUSCA SERIAL\nNúmeros primos encontrados na matriz: %u\n", countPrime );
    /** FIM BUSCA SERIAL */

    /** Libera espaço alocado para a matriz */
    freeMatriz( matriz, lengthLine );
    return 0;
}

int** createMatriz( int line, int column ) {
  int i;
  int **matriz = malloc( line * sizeof( unsigned long ) );

  for( i = 0; i < line; i++ ) {
    matriz[ i ] = malloc( column * sizeof( int ) );
  }

  return matriz;
}

void fillMatriz( int **matriz, line, column ) {
  int i, j;

  for( i = 0; i < line; i++ ) {
    srand( clock() );
    for( j = 0; j < column; j++ ) {
      matriz[ i ][ j ] = rand() %MAXRANDOM;
    }
  }
}

void serialSearch(, int **matriz, int line, int column ) {
  int i, j;

  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
      countPrime += isPrime( matriz[ i ][ j ] );
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

void printMatriz( int **matriz, int line, int column ) {
  int i, j;

  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
      printf( " %5d ", matriz[ i ][ j ] );
    }
    printf( "\n" );
  }
}

void freeMatriz( int **matriz, int line ) {
  int i;
  for ( i = 0; i < line; i++ ) {
    free( matriz[ i ] );
  }
  free( matriz );
}
