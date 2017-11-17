#pragma once
#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

// Exemplo 1: Hello World! com PThreads
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *thread(void *vargp);

int main() 
{
    pthread_t tid;  // Estrutura que define uma thread
    printf("Hello World da thread principal!\n");
    
	// Cria uma thread com os atributos definidos em tid, opções padrão NULL,
	// thread é função que contém o código da thread e não há parametros de entrada (ou seja, NULL)
	pthread_create(&tid, NULL, thread, NULL); 

	// Espera a thread "tid" terminar e não captura seu valor de retorno retorno (NULL)
    pthread_join(tid, NULL);

	system("pause");

	// Retorno NULL da thread principal. Desnecessário.
    pthread_exit((void *)NULL);

	
}

void *thread(void *vargp)
{
    printf("Hello World da thread criada pela thread principal!\n"); 
    pthread_exit((void *)NULL);
}
// --- Fim Exemplo 1 ---


//// Exemplo 2: Escopo de variáveis
//#include <stdio.h>
//#include <stdlib.h>
//#include <pthread.h>
//
//int global;
//void *thr_func(void *arg);
//
//int main(void)
//{
//	pthread_t tid;
//	global = 20;
//	printf("Thread principal: %d\n", global);
//	pthread_create(&tid, NULL, thr_func, NULL);
//	pthread_join(tid, NULL);
//	printf("Thread principal: %d\n", global);
//	system("pause");
//	return 0;
//}
//
//void *thr_func(void *arg)
//{
//	global = 40;
//	printf("Novo thread: %d\n", global);
//	return NULL;
//}
//// --- Fim Exemplo 2 ---

//// --- Exemplo 3: Multiplas Threads com parâmetro
//#include <stdio.h>
//#include <stdlib.h>
//#include <pthread.h>
//#define NUM_THREADS 5 
//
//void *PrintHello(void *threadid)
//{ 
//	int tid; 
//	tid = (int)threadid; 
//	printf("Hello World! It's me, thread #%d!\n", tid); 
//	pthread_exit(NULL); 
//} 
//
//int main (int argc, char *argv[])
//{ 
//	pthread_t threads[NUM_THREADS]; 
//	int rc, t; 
//
//	for(t=0; t<NUM_THREADS; t++)
//	{ 
//		printf("In main: creating thread %d\n", t); 
//		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t); 
//		if (rc)
//		{ 
//			printf("ERROR code is %d\n", rc); 
//			exit(-1); 
//		} 
//	} 
//
//	system("pause");
//
//	pthread_exit(NULL); 
//}  
//// --- Fim Exemplo 3 ---

//// --- Exemplo 4: Usando mutexes
//#include <stdio.h>
//#include <stdlib.h>
//#include <pthread.h>
//
//#define TAM 1000
//#define NTHREADS 10
//
//int a[TAM];
//int global_index = 0;
//int sum = 0;
//pthread_mutex_t mutex1; // Declaração do mutex
//
//void *slave (void *nenhum)
//{
//	int local_index, partial_sum = 0;
//
//	do {
//		
//		pthread_mutex_lock(&mutex1);
//		local_index = global_index;
//		global_index++;
//		pthread_mutex_unlock(&mutex1);
//
//		if (local_index < TAM) partial_sum += a[local_index];
//
//	} while (local_index < TAM);
//
//	pthread_mutex_lock(&mutex1);
//	sum += partial_sum;
//	pthread_mutex_unlock(&mutex1);
//
//	return(NULL);
//
//}
//
//main() 
//{
//	int i;
//	pthread_t thread [NTHREADS];
//
//	pthread_mutex_init(&mutex1, NULL);
//
//	for (i = 0; i < TAM; i++) a[i] = i+1;
//
//	for (i = 0; i < NTHREADS; i++)
//		if (pthread_create(&thread[i], NULL, slave, NULL) != 0)  
//		{
//			perror("Pthread_create falhou");
//			exit(1); 
//		}
//
//	for (i = 0; i < NTHREADS; i++)
//		if (pthread_join(thread[i], NULL) != 0) 
//		{
//			perror("Pthread_join falhou");
//			exit(1); 
//		}
//
//	printf("A soma eh %d \n", sum);
//
//	system("pause");
//}
//// --- Fim do Exemplo 4 ---