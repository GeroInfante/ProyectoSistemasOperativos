#include "colaDeMensaje.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

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

        srand(time(NULL));
        int vip, espera;

        ordenCliente.hamburguesa = 0;
        ordenCliente.vegano = 0;
        ordenCliente.papas = 0;
        vip = 0;

        if(rand() % 2 == 0 && rand() % 3 == 0)
            vip = 1;

        if(msgrcv(msqid, &ordenCliente, SIZEORDEN, COLAESPERA, IPC_NOWAIT) != -1)
        {
            //Maxima cantidad de cosas que puede pedir es 3 por tipo

            ordenCliente.hamburguesa = rand() % 4;
            ordenCliente.vegano = rand() % 4;
            ordenCliente.papas = rand() % 4;
            ordenCliente.PID = getpid();
            ordenCliente.entrega = 'N';

            if(ordenCliente.hamburguesa == 0 && ordenCliente.vegano == 0 && ordenCliente.papas == 0)
            {//Si no pidio nada, pide hamburguesa
                ordenCliente.hamburguesa = 1;
            }

            //Envio a Dispacher:
            if(vip)
            {
                ordenCliente.tipo = CLIENTEVIP;
                printf("\033[31mClienteVIP:\033[0m |H: %d|V: %d|P: %d, pid: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas, getpid());
                printf("\033[31mClienteVIP\033[0m envia su pedido a dispacher\n");
                msgsnd(msqid, &ordenCliente, SIZEORDEN, 0);
                sleep(1);
            }
            else
            {
                ordenCliente.tipo = CLIENTE;
                printf("\033[31mCliente:\033[0m |H: %d|V: %d|P: %d, pid: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas, getpid());
                msgsnd(msqid, &ordenCliente, SIZEORDEN, 0);
                printf("\033[31mCliente\033[0m envia su pedido a dispacher\n");
                sleep(1);
            }

            //Me pongo a esperar por lo que pedi:
            espera = ordenCliente.hamburguesa + ordenCliente.vegano + ordenCliente.papas;
            for(int i = 0; i < espera; i++)
            {
                msgrcv(msqid, &ordenCliente, SIZEORDEN, getpid(), 0);
                if(ordenCliente.entrega == 'H')
                    printf("\033[31mCliente\033[0m recibe hamburguesa, pid: %d\n", getpid());
                if(ordenCliente.entrega == 'V')
                    printf("\033[31mCliente\033[0m recibe vegano, pid: %d\n", getpid());
                if(ordenCliente.entrega == 'P')
                    printf("\033[31mCliente\033[0m recibe papas, pid: %d\n", getpid());
                sleep(1);
            }
            printf("\033[32mCliente termina, pid: %d\033[0m\n", getpid());
        }else{
            printf("\033[31mCliente\033[0m se va, hay mucha gente en la cola de espera\n");
            sleep(1);
        }
    }

    return 0;
}
