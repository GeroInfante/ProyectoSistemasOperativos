#include <stdio.h>
#include "colaDeMensaje.h"
#include <unistd.h>

int main()
{
    int msqid;
    msqid = conectaCola();
    if(msqid == -1)
    {
        printf("No se pudo conectar a la cola de mensajes\n");
        return -1;
    }

    struct orden ordenCliente;

    while(1)
    {
        msgrcv(msqid, &ordenCliente, SIZEORDEN, ATENDERCONPRIORIDAD, 0);
           
        printf("\033[4;37mDispacher recibido\033[0m, |H: %d|V: %d|P: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas);
        if(ordenCliente.hamburguesa != 0)
        {
            for(int i = 0; i < ordenCliente.hamburguesa; i++)
            {
                ordenCliente.tipo = EMPLEADOHAMBURGUESA;
                printf("\033[4;37mDispacher\033[0m-> Avisa a empleadoH\n");
                msgsnd(msqid, &ordenCliente, SIZEORDEN, 0);    
                sleep(1);
            }
        }
        if(ordenCliente.vegano != 0)
        {
            for(int i = 0; i < ordenCliente.vegano; i++)
            {
                ordenCliente.tipo = EMPLEADOVEGANO;
                printf("\033[4;37mDispacher\033[0m-> Avisa a empleadoV\n");
                msgsnd(msqid, &ordenCliente, SIZEORDEN, 0);
                sleep(1);
            }
        }
        if(ordenCliente.papas != 0)
        {
            for(int i = 0; i < ordenCliente.papas; i++)
            {
                ordenCliente.tipo = EMPLEADOPAPAS;
                printf("\033[4;37mDispacher\033[0m-> Avisa a empleadoP\n");
                msgsnd(msqid, &ordenCliente, SIZEORDEN, 0);
                sleep(1);
            }
        }
    }

    return 0;
}