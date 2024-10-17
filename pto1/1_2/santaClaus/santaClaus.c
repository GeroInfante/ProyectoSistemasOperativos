#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* santa(void* arg);
void* reno(void* arg);
void* elfo(void* arg);

sem_t despertar, hay_reno, renos, elfos, santaReno, santaElfo;
pthread_mutex_t mutexReno = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexElfo = PTHREAD_MUTEX_INITIALIZER;


int main()
{
	//Inicializacion de semaforos:
	sem_init(&despertar, 0, 0);
	sem_init(&hay_reno, 0, 0);
	sem_init(&renos, 0, 9);
	sem_init(&elfos, 0, 3);
	sem_init(&santaReno, 0, 0);
	sem_init(&santaElfo, 0, 0);

	
	//Creacion de hilos:
	
	pthread_t santaH;
	pthread_t elfosH[10];
	pthread_t renosH[9];
	
	int i;
	
	pthread_create(&santaH, NULL, santa, NULL);
	for(i = 0; i < 10; i++)
	{
		pthread_create(&elfosH[i], NULL, elfo, NULL);
	}
	
	for(i = 0; i < 9; i++)
	{
		pthread_create(&renosH[i], NULL, reno, NULL);
	}
	
	//Espero a los hilos terminar;
	
	pthread_join(santaH, NULL);
	for(i = 0; i < 10; i++)
	{
		pthread_join(elfosH[i], NULL);
	}
	
	for(i = 0; i < 9; i++)
	{
		pthread_join(renosH[i], NULL);
	}
	
	sem_destroy(&despertar);
	sem_destroy(&hay_reno);
	sem_destroy(&renos);
	sem_destroy(&elfos);
	sem_destroy(&santaReno);
	sem_destroy(&santaElfo);
	
	return 0;
	
}

void* santa(void* arg)
{
	while(1)
	{
		sem_wait(&despertar);
		printf("Santa se despierta\n");
		if( sem_trywait(&hay_reno) == 0)//Llego el utlimo reno a la puerta de santa y tiene prioridad para ser atendido
		{
			printf("Preparando el trineo\n");
			for(int i = 0; i < 9; i++)
			{
				sem_post(&santaReno);
				printf("Renos atados: %d\n", i+1);
			}
			for(int i = 0; i < 9; i++)
			{
				sem_post(&renos);
			}
		}else
		{
			for(int i = 0; i < 3; i++)
			{
				sem_post(&santaElfo);
				printf("Elfos ayudados por santa: %d\n", i);
				
			}
			for(int i = 0; i < 3; i++)
			{
				sem_post(&elfos);
			}
		}
		printf("Santa se va a dormir");
	}
}

void* reno(void* arg)
{
	while(1)
	{
		printf("Reno volviendo de vacaciones\n");
		pthread_mutex_lock(&mutexReno);
		sem_wait(&renos);
		if(sem_trywait(&renos) == -1)//Verifica ser el ultimo reno en volver de vacaciones
		{
			printf("\nReno llama a santa\n\n");
			sem_post(&hay_reno);
			sem_post(&despertar);//Si lo es despierta a santa, avisando primero
		}else
		{
			sem_post(&renos);
		}
		pthread_mutex_unlock(&mutexReno);
		printf("Reno volvio de vacaciones y espera a que lo ate santa\n");
		sem_wait(&santaReno);
		sleep(2);
	}
}

void* elfo(void* arg)
{
	while(1)
	{
		printf("Elfo haciando juguetes\n");
		if(rand() % 2 == 0)//posibilidad que tenga problemas o no
		{
			printf("Elfo con problemas para armar juguete\n");
			pthread_mutex_lock(&mutexElfo);
			sem_wait(&elfos);
			if(sem_trywait(&elfos) == -1)
			{
				printf("\nElfo llama a santa\n\n");
				sem_post(&despertar);
			}else
			{
				sem_post(&elfos);
			}
			pthread_mutex_unlock(&mutexElfo);
			printf("Elfo espera a que Santa lo ayude\n");
			sem_wait(&santaElfo);
		}
		sleep(2);
	}
}
