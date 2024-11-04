#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>

#define EXT_LECTURA 0
#define EXT_ESCRITURA 1
#define SIZEORDEN sizeof(struct orden)
#define SIZECONFIRMACION sizeof(int)


//Procesos:
void empleadoHamburguesas();
void empleadoVegano();
void cliente();
void atender();

//Hilos
void* empleadosPapas(void* arg);

struct orden
{
    int hamburguesa;
    int vegano;
    int papas;
};


int fdHamburguesaOrden[2]; //Pipes para comunicacion entre empleado de hamburguesas y despachador
int fdVeganoOrden[2]; //Pipes para comunicacion entre empleado vegano y despachador
int fdPapasOrden[2]; //Pipes para comunicacion entre empleado de papas y despachador

//Pipes para comunicacion entre despachador y cliente
int fdOrdenCliente[2], fdOrdenClienteVIP[2];
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
    pipe(fdOrdenClienteVIP);

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

    for(int i = 0; i < 5; i++)
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
    close(fdOrdenClienteVIP[EXT_ESCRITURA]);

    close(fdEsperaHamburguesa[EXT_LECTURA]);
    close(fdEsperaVegano[EXT_LECTURA]);
    close(fdEsperaPapas[EXT_LECTURA]);
    
    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);

    //Se va a atender a los clientes:
    atender();

    
    for(int i = 0; i < 7; i++)
    {
        wait(NULL);
    }
    return 0;   
}



void atender()
{
    struct orden ordenCliente;
    ordenCliente.hamburguesa = 0;
    ordenCliente.vegano = 0;
    ordenCliente.papas = 0;

    while(1)
    {
        int rturn;
        rturn = 0;
        fcntl(fdOrdenClienteVIP[EXT_LECTURA], F_SETFL, O_NONBLOCK);//Hace que el read no bloquee el proceso si no hay nada que leer en los VIPS
        int i;
        //Toma pedidos de los clientes:
        if((i = read(fdOrdenClienteVIP[EXT_LECTURA], &ordenCliente, SIZEORDEN)) != -1);
        else
            if((i =read(fdOrdenCliente[EXT_LECTURA], &ordenCliente, SIZEORDEN)) != -1);
        if(i > 0)
        {
            printf("\033[4;37mDispacher recibido\033[0m, |H: %d|V: %d|P: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas);
            //Determina de que tipo es el pedido y lo envia al empleado correspondiente
            if(ordenCliente.hamburguesa != 0)
            {
                for(int i = 0; i< ordenCliente.hamburguesa; i++)
                {
                    printf("\033[4;37mDispacher\033[0m-> Avisa a empleadoH\n");
                    write(fdHamburguesaOrden[EXT_ESCRITURA], &ordenCliente.hamburguesa, SIZECONFIRMACION);
                    sleep(1);
                }
            }
            if(ordenCliente.vegano != 0)
            {
                for(int i = 0; i < ordenCliente.vegano; i++)
                {
                    printf("\033[4;37mDispacher\033[0m-> Avisa a empleadoV\n");
                    write(fdVeganoOrden[EXT_ESCRITURA], &ordenCliente.vegano, SIZECONFIRMACION);
                    sleep(1);
                }
            }
            if(ordenCliente.papas != 0)
            {
                for(int i = 0; i < ordenCliente.papas; i++)
                {
                    printf("\033[4;37mDispacher\033[0m-> Avisa a empleadoP\n");
                    write(fdPapasOrden[EXT_ESCRITURA], &ordenCliente.papas, SIZECONFIRMACION);
                    sleep(1);
                }
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
    close(fdOrdenClienteVIP[EXT_LECTURA]);

    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);


    struct orden ordenCliente;

    while(1){
        srand(time(NULL));
        int vip;

        ordenCliente.hamburguesa = 0;
        ordenCliente.vegano = 0;
        ordenCliente.papas = 0;
        vip = 0;

        if(rand() % 5 == rand() % 9 )
        {
            printf("\033[32mCliente\033[0m-> se va, mucha gente en la cola de espera para pedir\n");
            sleep(1);
        }else
        {
            if(rand() % 2 == 0 && rand() % 3 == 0)
                vip = 1;

            //Maxima cantidad de cosas que puede pedir es 3 por tipo

            ordenCliente.hamburguesa = rand() % 4;
            ordenCliente.vegano = rand() % 4;
            ordenCliente.papas = rand() % 4;

            if(ordenCliente.hamburguesa == 0 && ordenCliente.vegano == 0 && ordenCliente.papas == 0)
            {//Si no pidio nada, pide hamburguesa
                ordenCliente.hamburguesa = 1;
            }
            if(vip)
            {
                //Envio orden al despachador 
                printf("\033[31mClienteVIP:\033[0m |H: %d|V: %d|P: %d, pid: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas, getpid());
                write(fdOrdenClienteVIP[EXT_ESCRITURA], &ordenCliente, SIZEORDEN);
                printf("\033[31mClienteVIP\033[0m envia su pedido a dispacher\n");
                sleep(1);
            }else
            {
                //Envio orden al despachador 
                printf("\033[31mCliente:\033[0m |H: %d|V: %d|P: %d, pid: %d\n", ordenCliente.hamburguesa, ordenCliente.vegano, ordenCliente.papas, getpid());
                write(fdOrdenCliente[EXT_ESCRITURA], &ordenCliente, SIZEORDEN);
                printf("\033[31mCliente\033[0m envia su pedido a dispacher\n");
                sleep(1);
            }
            //Me pongo a esperar por lo que pedi:
            if(ordenCliente.hamburguesa != 0)
            {
                for(int i = 0; i < ordenCliente.hamburguesa; i++)
                {
                    read(fdEsperaHamburguesa[EXT_LECTURA], &ordenCliente.hamburguesa, SIZECONFIRMACION);
                    printf("\033[31mCliente\033[0m recibe hamburguesa, pid: %d\n", getpid());
                }
            }
            if(ordenCliente.vegano != 0)
            {
                for(int i = 0; i < ordenCliente.vegano; i++)
                {
                    read(fdEsperaVegano[EXT_LECTURA], &ordenCliente.vegano, SIZECONFIRMACION);
                    printf("\033[31mCliente\033[0m recibe vegano, pid: %d\n", getpid());
                }
            }
            if(ordenCliente.papas != 0)
            {
                for(int i = 0; i < ordenCliente.papas; i++)
                {
                    read(fdEsperaPapas[EXT_LECTURA], &ordenCliente.papas, SIZECONFIRMACION);
                    printf("\033[31mCliente\033[0m recibe papas, pid: %d\n", getpid());
                }
            }
            printf("\033[32mCliente termina, pid: %d\033[0m\n", getpid());
            sleep(1);
        }

    }
    exit(1);
}


void empleadoHamburguesas()
{
    //Cierro extremos de pipe que no utiliza:
    //Externos
    close(fdOrdenCliente[EXT_ESCRITURA]);
    close(fdOrdenCliente[EXT_LECTURA]);
    close(fdOrdenClienteVIP[EXT_ESCRITURA]);
    close(fdOrdenClienteVIP[EXT_LECTURA]);

    close(fdVeganoOrden[EXT_ESCRITURA]);
    close(fdVeganoOrden[EXT_LECTURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_LECTURA]);

    close(fdPapasOrden[EXT_ESCRITURA]);
    close(fdPapasOrden[EXT_LECTURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_LECTURA]);

    //Propios
    close(fdHamburguesaOrden[EXT_ESCRITURA]);
    close(fdEsperaHamburguesa[EXT_LECTURA]);

    int confirmacion = 0;
    while(1)
    {
        //Espero orden de hamburguesas
        read(fdHamburguesaOrden[EXT_LECTURA], &confirmacion, SIZECONFIRMACION);
        printf("\033[36mEmpleadoH\033[0m-> prepara Hamburguesa\n");
        sleep(1);
        printf("\033[36mEmpleadoH\033[0m-> termina hamburguesa\n");
        write(fdEsperaHamburguesa[EXT_ESCRITURA], &confirmacion, SIZECONFIRMACION);
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
    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaHamburguesa[EXT_LECTURA]);

    close(fdPapasOrden[EXT_ESCRITURA]);
    close(fdPapasOrden[EXT_LECTURA]);
    close(fdEsperaPapas[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_LECTURA]);

    //Propios
    close(fdVeganoOrden[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_LECTURA]);

    int confirmacion = 0;
    while(1)
    {
        read(fdVeganoOrden[EXT_LECTURA], &confirmacion, SIZECONFIRMACION);
        printf("\033[35mEmpleadoV\033[0m-> prepara vegano\n");
        sleep(1);
        printf("\033[35mEmpleadoV\033[0m-> termina vegano\n");
        write(fdEsperaVegano[EXT_ESCRITURA], &confirmacion, SIZECONFIRMACION);
    }
    exit(1);
}

void* empleadosPapas(void* arg)
{
    //Cerramos pipes que no son utilizados
    //Externos
    close(fdOrdenCliente[EXT_ESCRITURA]);
    close(fdOrdenCliente[EXT_LECTURA]);
    close(fdOrdenClienteVIP[EXT_ESCRITURA]);
    close(fdOrdenClienteVIP[EXT_LECTURA]);

    close(fdVeganoOrden[EXT_ESCRITURA]);
    close(fdVeganoOrden[EXT_LECTURA]);
    close(fdEsperaVegano[EXT_ESCRITURA]);
    close(fdEsperaVegano[EXT_LECTURA]);

    close(fdHamburguesaOrden[EXT_ESCRITURA]);
    close(fdHamburguesaOrden[EXT_LECTURA]);
    close(fdEsperaHamburguesa[EXT_ESCRITURA]);
    close(fdEsperaHamburguesa[EXT_LECTURA]);

    //Propios
    close(fdPapasOrden[EXT_ESCRITURA]);
    close(fdEsperaPapas[EXT_LECTURA]);

    int confirmacion = 0;

    while(1)
    {
        read(fdPapasOrden[EXT_LECTURA], &confirmacion, SIZECONFIRMACION);
        printf("\033[33mEmpleadoP\033[0m-> prepara papas\n");
        sleep(1);
        printf("\033[33mEmpleadoP\033[0m-> termina papas\n");
        write(fdEsperaPapas[EXT_ESCRITURA], &confirmacion, SIZECONFIRMACION);
    }
}