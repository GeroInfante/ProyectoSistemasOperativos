#include "colaDeMensaje.h"
#include <stdio.h>
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
        msgrcv(msqid, &ordenCliente, SIZEORDEN, EMPLEADOPAPAS, 0);
        printf("\033[33mEmpleadoP\033[0m-> prepara papas\n");
        sleep(1);
        printf("\033[33mEmpleadoP\033[0m-> termina papas\n");
        ordenCliente.entrega = 'P';
        ordenCliente.tipo = ordenCliente.PID;
        msgsnd(msqid, &ordenCliente, SIZEORDEN, 0);
    }

    return 0;
}

