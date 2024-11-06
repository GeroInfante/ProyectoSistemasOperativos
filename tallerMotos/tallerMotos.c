#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t ruedaSem, chasisSem, motorSem, pintarSem, extraSem, secuenciaSem;

void* ruedas(void* param);
void* chasis(void* param);
void* motor(void* param);
void* pintorRojo(void* param);
void* pintorVerde(void* param);
void* extra(void* param);

void main()
{
	//Inicializacion de semaforos:
	sem_init(&ruedaSem, 0, 2);
	sem_init(&chasisSem, 0, 0);
	sem_init(&motorSem, 0, 0);
	sem_init(&pintarSem, 0, 0);
	sem_init(&extraSem, 0, 0);
	sem_init(&secuenciaSem, 0, 4);
	
	
	
	//Creacion de los hilos
	pthread_t tid[6];
	pthread_create(&tid[0], NULL, ruedas, NULL);
	pthread_create(&tid[1], NULL, chasis, NULL);
	pthread_create(&tid[2], NULL, motor, NULL);
	pthread_create(&tid[3], NULL, pintorRojo, NULL);
	pthread_create(&tid[4], NULL, pintorVerde, NULL);
	pthread_create(&tid[5], NULL, extra, NULL);
	
	
	for(int i=0; i<6; i++)
	{
		pthread_join(tid[i], NULL);
	}
	
	printf("TERMINA PROCESO MAIN\n");
}


void* ruedas(void* param)
{
	while(1)
	{
		sem_wait(&secuenciaSem);
		sem_wait(&ruedaSem);
		printf("R");
		sem_post(&chasisSem);
	}
}

void* chasis(void* param)
{
	while(1)
	{
		sem_wait(&chasisSem);
		sem_wait(&chasisSem);
		printf("C");
		sem_post(&motorSem);
	}
}

void* motor(void* param)
{
	while(1)
	{
		sem_wait(&motorSem);
		printf("M");
		sem_post(&pintarSem);
	}
}

void* pintorVerde(void* param)
{
	while(1)
	{
		sem_wait(&pintarSem);
		printf("P_verde");
		sem_post(&extraSem);
		sem_post(&ruedaSem);
		sem_post(&ruedaSem);
	}
}
void* pintorRojo(void* param)
{
	while(1)
	{
		sem_wait(&pintarSem);
		printf("P_rojo");
		sem_post(&extraSem);
		sem_post(&ruedaSem);
		sem_post(&ruedaSem);
	}
}

void* extra(void* param)
{
	while(1)
	{
		sem_wait(&extraSem);
		sem_wait(&extraSem);
		printf("E ");
		sem_post(&secuenciaSem);
		sem_post(&secuenciaSem);
		sem_post(&secuenciaSem);
		sem_post(&secuenciaSem);
	}
}
