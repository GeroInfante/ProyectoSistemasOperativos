#include "colaDeMensaje.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    //proceso principal crea la cola de mensajes
    int msqid = msgget(KEY, 0666 | IPC_CREAT);
	struct orden msj;

    if(msqid == -1)
	{
		printf("No se pudo crear cola de mensajes\n");
		return 1;
	}

    //Determinamos el numero maximo de clientes en cola de espera antes de que se vaya un cliente es 10:
    msj.tipo = COLAESPERA;
    for(int i = 0; i < 10; i++)
    {
        msgsnd(msqid, &msj, SIZEORDEN, 0);
    }
    //Creo procesos clientes y empleados:

    for(int i = 0; i < 12; i++)
    {
        if(fork() == 0)
        {
            execv("./cliente", NULL);
            exit(1);
        }
        sleep(1);
    }

    if(fork() == 0)
    {
        execv("./empleadoHamburguesa", NULL);
        exit(1);
    }
    if(fork() == 0)
    {
        execv("./empleadoVegano", NULL);
        exit(1);
    }
    if(fork() == 0)
    {
        execv("./empleadoPapas", NULL);
        exit(1);
    }
    if(fork() == 0)
    {
        execv("./empleadoPapas", NULL);
        exit(1);
    }
    if(fork() == 0)
    {
        execv("./despachador", NULL);
        exit(1);
    }

    return 0;
}