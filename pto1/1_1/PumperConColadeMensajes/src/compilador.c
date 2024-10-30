#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("gcc cliente.c -o ../bin/cliente");
    system("gcc empleadoHamburguesa.c -o ../bin/empleadoHamburguesa");
    system("gcc empleadoVegano.c -o ../bin/empleadoVegano");
    system("gcc empleadoPapas.c -o ../bin/empleadoPapas");
    system("gcc despachador.c -o ../bin/despachador");
    system("gcc PumperNic-ColaMensajes.c -o ../bin/main");
    return 0;
}