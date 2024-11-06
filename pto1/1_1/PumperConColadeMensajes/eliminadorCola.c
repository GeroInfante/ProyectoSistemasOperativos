#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

void main()
{
	int msgid;
	msgid = msgget(1234, 0666); //se conecta a cola de mensajes
	msgctl(msgid, IPC_RMID, NULL); //Elimina la cola de mensajes al finalizar
	printf("Listo\n");
	
}
