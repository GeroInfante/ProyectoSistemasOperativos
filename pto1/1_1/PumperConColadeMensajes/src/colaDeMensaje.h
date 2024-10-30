#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>


#define KEY ((key_t)1234)
#define SIZEORDEN sizeof(struct orden) - sizeof(long)

#define ATENDERCONPRIORIDAD -2
#define CLIENTEVIP 1
#define CLIENTE 2
#define EMPLEADOHAMBURGUESA 3
#define EMPLEADOVEGANO 4
#define EMPLEADOPAPAS 5


struct orden
{//Cuando cliente quiere alguna de estas pone su valor en 1, si no en 0
    long tipo;
    int hamburguesa;
    int vegano;
    int papas;
    char entrega;//H o V o P
    int PID;
};

int conectaCola()
{
    int msqid = msgget(KEY, 0666);
    return msqid;
}