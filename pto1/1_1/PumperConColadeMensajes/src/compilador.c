#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("gcc cliente.c -o cliente");
    system("gcc empleadoHamburguesa.c -o empleadoHamburguesa");
    system("gcc empleadoVegano.c -o empleadoVegano");
    system("gcc empleadoPapas.c -o empleadoPapas");
    system("gcc despachador.c -o despachador");
    system("gcc PumperNic-ColaMensajes.c -o main");

    printf("Compilacion exitosa\nEjecute el programa con el comando: ./main\n");

    return 0;
}