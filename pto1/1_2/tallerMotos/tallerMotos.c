#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t hacerRueda, ruedaLista, chasisListo, pintar, terminada;

void* ruedas(void* param);
void* chasis(void* param);
void* motor(void* param);
void* pintorRojo(void* param);
void* pintorVerde(void* param);
void* extra(void* param);

void main()
{
	//Inicializacion de semaforos:
	sem_init(&hacerRueda, 0, 2);
	sem_init(&ruedaLista, 0, 0);
	sem_init(&chasisListo, 0, 0);
	sem_init(&pintar, 0, 0);
	sem_init(&terminada, 0, 0);
	
	
	
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
		sem_wait(&hacerRueda);
		printf("R");
		sem_post(&ruedaLista);
	}
}

void* chasis(void* param)
{
	while(1)
	{
		sem_wait(&ruedaLista);
		sem_wait(&ruedaLista);
		printf("C");
		sem_post(&chasisListo);
	}
}

void* motor(void* param)
{
	while(1)
	{
		sem_wait(&chasisListo);
		printf("M");
		sem_post(&pintar);
	}
}

void* pintorVerde(void* param)
{
	while(1)
	{
		sem_wait(&pintar);
		printf("P_verde");
		sem_post(&terminada);
	}
}
void* pintorRojo(void* param)
{
	while(1)
	{
		sem_wait(&pintar);
		printf("P_rojo");
		sem_post(&terminada);
	}
}

void* extra(void* param)
{
	while(1)
	{
		sem_wait(&terminada);
		sem_post(&hacerRueda);
		sem_post(&hacerRueda);
		sem_wait(&terminada);
		printf("E ");
		sem_post(&hacerRueda);
		sem_post(&hacerRueda);
	}
}
