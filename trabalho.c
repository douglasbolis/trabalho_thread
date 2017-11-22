/**
 * trabalho.c
 * Purpose: Calcular a quantidade de números primos em uma determinada matriz de forma serial e paralela.
 * Na busca de forma paralela a matriz deverá ser dividida em novas matrizes para a busca ser efetuada.
 * 
 * @author Douglas Bolis Lima <douglasbolislima@gmail.com>
 * @version 0.3.5 17/11/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

#define FALSE 0
#define TRUE 1

/**
 * Características da matriz.
 */
#define TAM_LINHA 15000
#define TAM_COLUNA 15000
#define MAX_RANDOM 29999
// #define TAM_LINHA 9
// #define TAM_COLUNA 9
// #define MAX_RANDOM 19
/**
 * Seed para gerar os números randômicos.
 */
#define SEED 7
// #define SEED ( unsigned )time( NULL )
/**
 * Características dos macroblocos.
 */
#define NUM_THREADS 4
#define NUM_MB_LINES 150
#define NUM_MB_COLUMNS 150

/**
 * Estrutura dos structs.
 */
// Matriz.
typedef struct {
  int **data, lines, columns;
} Matriz;
// Macrobloco.
typedef struct {
  int startLine, startColumn, endLine, endColumn;
} Macrobloco;

/**
 * Variáveis globais.
 */
// Regiões críticas.
Matriz *matriz;
Macrobloco *macrobloco;
unsigned int primeNumberCounter = 0;
unsigned int primeNumberCounterParallel = 0;
unsigned int gerenciadorMacrobloco = 0;
unsigned int macroblocoCounter = 0;
pthread_mutex_t mutexMacrobloco;
pthread_mutex_t mutexPrimeNumber;

/**
 * Assinatura dos métodos.
 */
// Verifica se o número é primo.
unsigned int isPrime( int );
// Cria e retorna uma matriz.
Matriz* createMatriz( int, int );
// Preenche a matriz com números aleatório de 0 até MAX_RANDOM.
void fillMatriz();
// Divide a matriz em macroblocos.
Macrobloco* splitMatriz();
// Imprime os elementos da matriz.
void printMatriz();
// Realiza a busca serial na matriz.
void serialSearch();
// Realiza a busca serial na matriz.
void parallelSearch();
// Faz a busca paralela na matriz.
void *functionThread( void* );
// Retorna um macrobloco disponível.
void* findMacrobloco();
// Imprime os elementos do macrobloco.
void printMacrobloco( Macrobloco* );
// Libera o espaço alocado para a matriz
int freeMatriz();

/**
 * Método principal.
 *
 * @param argc Número de argumentos.
 * @param argv Ponteiro para a lista de argumentos.
 * @return Retorna o código indicativo se o programa foi executado corretamente.
 */
int main( int argc, char **argv ) {
  // Iniciando os mutexes;
	pthread_mutex_init( &mutexMacrobloco, NULL );
	pthread_mutex_init( &mutexPrimeNumber, NULL );

  // Preparando a matriz para a relalização das buscas.
  // criando a matriz
  matriz = createMatriz( TAM_LINHA, TAM_COLUNA );
  // adicionando números aleatórios
  fillMatriz();

  // Dividindo a matriz em macroblocos
  macrobloco = splitMatriz();

  // Imprimindo matriz
  // printMatriz();

  // INÍCIO BUSCA SERIAL
  serialSearch();
  printf( "\nBUSCA SERIAL\nNúmeros primos encontrados na matriz: %u\n", primeNumberCounter );
  // FIM BUSCA SERIAL

  // INÍCIO BUSCA PARALELA
  parallelSearch();
  printf( "\nBUSCA PARALELA\nNúmeros primos encontrados na matriz: %u\n", primeNumberCounterParallel );
  // FIM BUSCA PARALELA

  // Libera espaço alocado para a matriz
  freeMatriz();
  return 0;
}

/**
 * Cria e retorna uma matriz.
 *
 * @param {int} lines Número de linhas da matriz.
 * @param {int} columns Número de colunas da matriz.
 * @return {Matriz*} Ponteiro para os dados da matriz criada.
 */
Matriz* createMatriz( int lines, int columns ) {
  int i;
  Matriz *matriz = malloc( sizeof( Matriz* ) );

  matriz->data = malloc( lines * sizeof( int* ) );

  for ( i = 0; i < lines; i++ ) {
    matriz->data[ i ] = malloc( columns * sizeof( int ) );
  }

  matriz->lines = lines;
  matriz->columns = columns;

  return matriz;
}

/**
 * Preenche a matriz com números aleatório de 0 até MAX_RANDOM.
 *
 * @return {void} Método sem retorno.
 */
void fillMatriz() {
  int i, j;

  srand( SEED );
  for ( i = 0; i < matriz->lines; i++ ) {
    for ( j = 0; j < matriz->columns; j++ ) {
      /**
      * ( MAX_RANDOM + 1 ) para que o número máximo definido também possa ser gerado.
      */
      matriz->data[ i ][ j ] = rand() % ( MAX_RANDOM + 1 );
    }
  }
}

/**
 * Divide a matriz em vários macroblocos.
 *
 * @return {Macrobloco*} Ponteiro para o vetor de macroblocos criado.
 */
Macrobloco* splitMatriz() {
  Macrobloco *mb;
  int macroblocoLine, macroblocoColumn, macroblocoNumber = 0;

  macroblocoCounter = ( matriz->lines * matriz->columns ) / ( NUM_MB_LINES * NUM_MB_COLUMNS );

  mb = malloc( macroblocoCounter * sizeof( Macrobloco ) );
	
	for ( macroblocoLine = 0; macroblocoLine < matriz->lines; macroblocoLine += NUM_MB_LINES ) {
		for ( macroblocoColumn = 0; macroblocoColumn < matriz->columns; macroblocoColumn += NUM_MB_COLUMNS, macroblocoNumber += 1 ) {
			mb[ macroblocoNumber ].startLine = macroblocoLine;
			mb[ macroblocoNumber ].endLine = macroblocoLine + NUM_MB_LINES;
			mb[ macroblocoNumber ].startColumn = macroblocoColumn;
			mb[ macroblocoNumber ].endColumn = macroblocoColumn + NUM_MB_COLUMNS;
		}
  }

  return mb;
}

/**
 * Realiza a busca serial na matriz.
 *
 * @return {void} Método sem retorno.
 */
void serialSearch() {
  int i, j;


  for ( i = 0; i < matriz->lines; i++ ) {
    for ( j = 0; j < matriz->columns; j++ ) {
      primeNumberCounter += isPrime( matriz->data[ i ][ j ] );
    }
  }
}

/**
 * Realiza a busca paralela na matriz.
 *
 * @return {void} Método sem retorno.
 */
void parallelSearch() {
  int threadsCounter, responseCodeThread;
	pthread_t threads[ NUM_THREADS ];

  // Criando as threads e inicializando-as para já executarem suas buscas.
  for ( threadsCounter = 0; threadsCounter < NUM_THREADS; threadsCounter++ ) {
    responseCodeThread = pthread_create( &threads[ threadsCounter ], NULL, &functionThread, NULL );
    if ( responseCodeThread ) {
      printf( "ERROR code is %d\n", responseCodeThread );
      exit( -1 );
    }
  }
  // Esperando que as threads finalizem as buscas.
	for ( threadsCounter = 0; threadsCounter < NUM_THREADS; threadsCounter++ ) {
    pthread_join( threads[ threadsCounter ], NULL );
	}
}

/**
 * Método de execução da thread.
 * Quando a thread é criada e iniciada, a thread executará este método.
 *
 * @param {void*} nothing Dados que a thread pode acessar que não sejam globais.
 * @return {void} Método sem retorno.
 */
void *functionThread( void *nothing ) {
	Macrobloco* mb;
	int i, j;
  unsigned int primeNumberCounterLocal;

	while ( TRUE ) {
    // Região crítica para a thread pegar um dos macroblocos disponíveis para a busca.
    pthread_mutex_lock( &mutexMacrobloco );
    mb = ( Macrobloco* )findMacrobloco();
    pthread_mutex_unlock( &mutexMacrobloco );

		if ( mb == NULL ) {
      // Caso a thread não encontrar um macrobloco disponível ela será encerrada.
      pthread_exit( NULL );
    }

    // printMacrobloco( mb );
    // Zerando o contador local de números primos antes de percorrer o macrobloco.
    primeNumberCounterLocal = 0;

		//Verificando o macrobloco encontrado.
		for ( i = mb->startLine; i < mb->endLine; i++ ) {
			for ( j = mb->startColumn; j < mb->endColumn; j++ ) {
				primeNumberCounterLocal += isPrime( matriz->data[ i ][ j ] );
			}
    }

    // Região crítica para incrementar o contador de números primos global.
		pthread_mutex_lock( &mutexPrimeNumber );
    primeNumberCounterParallel += primeNumberCounterLocal;
		pthread_mutex_unlock( &mutexPrimeNumber );
	}
}

/**
 * Verifica se o número é primo.
 *
 * @param {int} n Número a ser verificado.
 * @return {int} O indicativo do número ser primo.
 */
unsigned int isPrime( int n ) {
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
 * @return {void} Método sem retorno.
 */
void printMatriz() {
  int i, j;

  printf( "MATRIZ %u X %u:\n\n", TAM_LINHA, TAM_COLUNA );

  for ( i = 0; i < matriz->lines; i++ ) {
    for ( j = 0; j < matriz->columns; j++ ) {
      printf( " %5d ", matriz->data[ i ][ j ] );
    }
    printf( "\n" );
  }
}

/**
 * Encontra um macrobloco disponível.
 *
 * @return {void*} O macrobloco encontrado.
 */
void* findMacrobloco() {
  return ( gerenciadorMacrobloco >= macroblocoCounter ) ? NULL : &macrobloco[ gerenciadorMacrobloco++ ];
}

/**
 * Imprime os elementos do macrobloco.
 *
 * @return {void} Método sem retorno.
 */
void printMacrobloco( Macrobloco *mb ) {
  int i, j, primos = 0;

  printf( "MACROBLOCO %u X %u:\n\n", NUM_MB_LINES, NUM_MB_COLUMNS );

  printf( "linhas: %u e %u\ncolunas: %u e %u\n", mb->startLine, mb->endLine, mb->startColumn, mb->endColumn );

  for ( i = mb->startLine; i < mb->endLine; i++ ) {
    for ( j = mb->startColumn; j < mb->endColumn; j++ ) {
      primos += isPrime( matriz->data[ i ][ j ] );
      printf( " %5d ", matriz->data[ i ][ j ] );
    }
    printf( "\n" );
  }

  printf( "Números primos no macrobloco: %u.\n", primos );
}

/**
 * Libera o espaço alocado para a estrutura da matriz.
 *
 * @return {int} Método sem retorno.
 */
int freeMatriz() {
  int i;
  for ( i = 0; i < matriz->lines; i++ ) {
    free( matriz->data[ i ] );
  }
  free( matriz->data );
  free( matriz );
  free( macrobloco );

  return 1;
}
