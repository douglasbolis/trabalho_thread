/**
 * trabalho.c
 * Purpose: Calcular a quantidade de números primos em uma determinada matriz de forma serial e paralela.
 * Na busca de forma paralela a matriz deverá ser dividida em novas matrizes para a busca ser efetuada.
 * 
 * @author Douglas Bolis Lima <douglasbolislima@gmail.com>
 * @version 0.2.2 24/10/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/**
 * Características da matriz.
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
// Verifica se o número é primo.
int isPrime( int );
// Cria e retorna uma matriz.
int** createMatrix( int, int );
// Preenche a matriz com números aleatório de 0 até MAXRANDOM.
void fillMatrix( int**, int, int );
// Imprime os elementos da matriz.
void printMatrix( int**, int, int );
// Faz a busca serial na matriz.
void serialSearch( int**, int, int );
// Libera o espaço alocado para a matriz
void freeMatrix( int**, int );

/**
 * Método principal.
 *
 * @param argc Número de argumentos.
 * @param argv Ponteiro para a lista de argumentos.
 * @return Retorna o código indicativo se o programa foi executado corretamente.
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

/**
 * Cria e retorna uma matriz.
 *
 * @param line Número de linhas da matriz.
 * @param column Número de colunas da matriz.
 * @return Ponteiro para a matriz criada.
 */
int** createMatrix( int line, int column ) {
  int i;
  int **matrix = malloc( line * sizeof( unsigned long ) );

  for( i = 0; i < line; i++ ) {
    matrix[ i ] = malloc( column * sizeof( int ) );
  }

  return matrix;
}

/**
 * Preenche a matriz com números aleatório de 0 até MAXRANDOM.
 *
 * @param matrix Ponteiro da matriz.
 * @param line Número de linhas que a matriz possui.
 * @param column Número de colunas que a matriz possui.
 * @return Método sem retorno.
 */
void fillMatrix( int **matrix, int line, int column ) {
  int i, j;

  srand( ( unsigned )time( NULL ) );
  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
       /**
        * ( MAXRANDOM + 1 ) para que o número máximo definido também possa ser gerado.
        */
      matrix[ i ][ j ] = rand() % ( MAXRANDOM + 1 );
    }
  }
}

/**
 * Faz a busca serial na matriz.
 *
 * @param matrix Ponteiro da matriz.
 * @param line Número de linhas que a matriz possui.
 * @param column Número de colunas que a matriz possui.
 * @return Método sem retorno.
 */
void serialSearch( int **matrix, int line, int column ) {
  int i, j;

  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
      countPrime += isPrime( matrix[ i ][ j ] );
    }
  }
}


/**
 * Verifica se o número é primo.
 *
 * @param n Número a ser verificado.
 * @return O indicativo do número ser primo.
 */
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

/**
 * Imprime os elementos da matriz.
 *
 * @param matrix Ponteiro da matriz.
 * @param line Número de linhas que a matriz possui.
 * @param column Número de colunas que a matriz possui.
 * @return Método sem retorno.
 */
void printMatrix( int **matrix, int line, int column ) {
  int i, j;

  for( i = 0; i < line; i++ ) {
    for( j = 0; j < column; j++ ) {
      printf( " %5d ", matrix[ i ][ j ] );
    }
    printf( "\n" );
  }
}

/**
 * Libera o espaço alocado para a matriz.
 *
 * @param matrix Ponteiro da matriz.
 * @param line Número de linhas que a matriz possui.
 * @return Método sem retorno.
 */
void freeMatrix( int **matrix, int line ) {
  int i;
  for ( i = 0; i < line; i++ ) {
    free( matrix[ i ] );
  }
  free( matrix );
}
