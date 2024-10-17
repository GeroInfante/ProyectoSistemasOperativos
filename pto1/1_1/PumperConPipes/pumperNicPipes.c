#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define EXT_LECTURA 0
#define EXT_ESCRITURA 1
#define SIZEORDEN sizeof(struct orden)
#define SIZECONFIRMACION sizeof(int)


//Procesos:
void empleadoHamburguesas();
void empleadoVegano();
void cliente();

//Hilos
void* empleadosPapas(void* arg);
void* atender(void* arg);

struct orden
{//Cuando cliente quiere alguna de estas pone su valor en 1, si no en 0
    int hamburguesa;
    int vegano;
    int papas;
};


int fdHamburguesaOrden[2]; //Pipes para comunicacion entre empleado de hamburguesas y despachador
int fdVeganoOrden[2]; //Pipes para comunicacion entre empleado vegano y despachador
int fdPapasOrden[2]; //Pipes para comunicacion entre empleado de papas y despachador

//Pipes para comunicacion entre despachador y cliente
int fdOrdenCliente[2];
int fdEsperaHamburguesa[2], fdEsperaVegano[2], fdEsperaPapas[2];

int main(int argc, char *argv[]){
    //Creo los pipes:
    //Pipes con empleado de hamburguesas
    pipe(fdHamburguesaOrden);

    //Pipes con empleado vegano
    pipe(fdVeganoOrden);

    //Pipes con empleado de papas
    pipe(fdPapasOrden);

    //Pipes con clientes
    pipe(fdOrdenCliente);
    
    pipe(fdEsperaHamburguesa);
    pipe(fdEsperaVegano);
    pipe(fdEsperaPapas);


    //Creacion de procesos hijos:
    if(fork() == 0)//Creamos empleado encargado de las hamburguesas
    {
        empleadoHamburguesas();
        exit(1);
    }

    if(fork() == 0)//Creamos empleado encargado de combo vegano
    {
        empleadoVegano();
        exit(1);
    }

    if(fork() == 0)//Creamos empleados encargado de las papas
    {
        pthread_t hiloPapas[2];
        pthread_create(&hiloPapas[0], NULL, empleadosPapas, NULL);
        pthread_create(&hiloPapas[1], NULL, empleadosPapas, NULL);
        
        pthread_join(hiloPapas[0], NULL);
        pthread_join(hiloPapas[1], NULL);
        exit(1);
    }

    for(int i = 0; i < 1; i++)
    {
        if(fork() == 0)//Creamos clientes
        {
            cliente();
            exit(1);
        }
        sleep(1);
    }

    //Cierre de extremos de pipe;
    close(fdHamburguesaOrden[EXT_LECTURA]);

    close(fdVeganoOrden[EXT_LECTURA]);

    close(fdPapasOrden[EXT_LECTURA]);

    close(fdOrdenCliente[EXT_ESCRITURA]);

    close(fdEsperaHamburguesa[EXT_LECTURA]);
    close(fdEsperaVegano[EXT_LECTURA]);
    close(fdEsperaPapas[EXT_LECTURA]);
    
    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);

    //Creo hilos para manejar la atencion de los clientes
    pthread_t hiloAtencion, hiloDespachador;

    pthread_create(&hiloAtencion, NULL, atender, NULL);
    //pthread_create(&hiloDespachador, NULL, despachar, NULL);

    pthread_join(hiloAtencion, NULL);
    //pthread_join(hiloDespachador, NULL);

    
    for(int i = 0; i < 7; i++)
    {
        wait(NULL);
    }
    return 0;   
}



void* atender(void* arg)
{
    struct orden ordenCliente;
    ordenCliente.hamburguesa = 0;
    ordenCliente.vegano = 0;
    ordenCliente.papas = 0;

    while(1)
    {

        //Toma pedidos de los clientes:
        if(read(fdOrdenCliente[EXT_LECTURA], &ordenCliente, SIZEORDEN) != 0){
            printf("Dispacher recibido, |H: %d|V: %d|P: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas);
            //Determina de que tipo es el pedido y lo envia al empleado correspondiente
            if(ordenCliente.hamburguesa == 1)
            {
                printf("D-> Leo hamburguesa\n");
                printf("D-> Avisa a empleadoH\n");
                write(fdHamburguesaOrden[EXT_ESCRITURA], &ordenCliente.hamburguesa, SIZECONFIRMACION);
                sleep(1);
            }
            if(ordenCliente.vegano == 1)
            {
                printf("D-> Leo Vegano\n");
                write(fdVeganoOrden[EXT_ESCRITURA], &ordenCliente.vegano, SIZECONFIRMACION);
                printf("D-> Avisa a empleadoV\n");
                sleep(1);
            }
            if(ordenCliente.papas == 1)
            {
                printf("D-> Leo Papas\n");
                write(fdPapasOrden[EXT_ESCRITURA], &ordenCliente.papas, SIZECONFIRMACION);
                printf("D-> Avisa a empleadoP\n");
                sleep(1);
            }
        }
    }
}


void cliente()
{
    //cierra extremos de pipe que no utiliza
    //Externos
    close(fdHamburguesaOrden[EXT_ESCRITURA]);
    close(fdHamburguesaOrden[EXT_LECTURA]);

    close(fdVeganoOrden[EXT_ESCRITURA]);
    close(fdVeganoOrden[EXT_LECTURA]);

    close(fdPapasOrden[EXT_ESCRITURA]);
    close(fdPapasOrden[EXT_LECTURA]);

    //Propios
    close(fdOrdenCliente[EXT_LECTURA]);
    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);


    struct orden ordenCliente;

    //while(1){
        srand(time(NULL));

        ordenCliente.hamburguesa = 0;
        ordenCliente.vegano = 0;
        ordenCliente.papas = 0;

        if(rand() % 2 == 0)
            ordenCliente.hamburguesa = 1;
        if(rand() % 2 == 0)
            ordenCliente.vegano = 1;
        if(rand() % 2 == 0)
            ordenCliente.papas = 1;

        printf("Cliente: |H: %d|V: %d|P: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas);
        //Envio orden al despachador 
        write(fdOrdenCliente[EXT_ESCRITURA], &ordenCliente, SIZEORDEN);
        printf("Cliente envia su pedido a dispacher\n");



        //ESPERA A QUE SE DESPACHE SU PEDIDO
        sleep(4);

    //}
    exit(1);
}


void empleadoHamburguesas()
{
    //Cierro extremos de pipe que no utiliza:
    //Externos
    close(fdOrdenCliente[EXT_ESCRITURA]);
    close(fdOrdenCliente[EXT_LECTURA]);

    close(fdVeganoOrden[EXT_ESCRITURA]);
    close(fdVeganoOrden[EXT_LECTURA]);

    close(fdPapasOrden[EXT_ESCRITURA]);
    close(fdPapasOrden[EXT_LECTURA]);

    close(fdEsperaHamburguesa[EXT_LECTURA]);
    close(fdEsperaVegano[EXT_LECTURA]);
    close(fdEsperaPapas[EXT_LECTURA]);
    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);


    //Propios
    close(fdHamburguesaOrden[EXT_ESCRITURA]);

    int confirmacion = 0;
    while(1)
    {
        //Espero orden de hamburguesas
        read(fdHamburguesaOrden[EXT_LECTURA], &confirmacion, SIZECONFIRMACION);
        printf("EmpleadoH prepara Hamburguesa\n");
        sleep(1);
    }
    exit(1);
}

void empleadoVegano()
{
    //Cierro extremos de pipe que no utiliza:
    //Externos
    close(fdOrdenCliente[EXT_LECTURA]);
    close(fdOrdenCliente[EXT_ESCRITURA]);

    close(fdHamburguesaOrden[EXT_ESCRITURA]);
    close(fdHamburguesaOrden[EXT_LECTURA]);

    close(fdPapasOrden[EXT_ESCRITURA]);
    close(fdPapasOrden[EXT_LECTURA]);

    close(fdEsperaHamburguesa[EXT_LECTURA]);
    close(fdEsperaVegano[EXT_LECTURA]);
    close(fdEsperaPapas[EXT_LECTURA]);
    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);

    //Propios
    close(fdVeganoOrden[EXT_ESCRITURA]);

    int confirmacion = 0;
    while(1)
    {
        read(fdVeganoOrden[EXT_LECTURA], &confirmacion, SIZECONFIRMACION);
        printf("EmpleadoV prepara vegano\n");
        sleep(1);
    }
    exit(1);
}

void* empleadosPapas(void* arg)
{
    //Cerramos pipes que no son utilizados
    //Externos
    close(fdOrdenCliente[EXT_ESCRITURA]);
    close(fdOrdenCliente[EXT_LECTURA]);

    close(fdVeganoOrden[EXT_ESCRITURA]);
    close(fdVeganoOrden[EXT_LECTURA]);

    close(fdHamburguesaOrden[EXT_ESCRITURA]);
    close(fdHamburguesaOrden[EXT_LECTURA]);

    //Propios
    close(fdPapasOrden[EXT_ESCRITURA]);

    int confirmacion = 0;

    while(1)
    {
        read(fdPapasOrden[EXT_LECTURA], &confirmacion, SIZECONFIRMACION);
        printf("EmpleadoP prepara papas\n");
        sleep(1);

    }
}