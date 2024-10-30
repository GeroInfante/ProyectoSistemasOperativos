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
        msgrcv(msqid, &ordenCliente, SIZEORDEN, EMPLEADOVEGANO, 0);
        printf("\033[35mEmpleadoV\033[0m-> prepara vegano\n");
        sleep(1);
        printf("\033[35mEmpleadoV\033[0m-> termina vegano\n");
        ordenCliente.entrega = 'V';
        ordenCliente.tipo = ordenCliente.PID;
        msgsnd(msqid, &ordenCliente, SIZEORDEN, 0);
    }

    return 0;
}

