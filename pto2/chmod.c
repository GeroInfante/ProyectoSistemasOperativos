#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
int main(int argc, char* argv[])
{
    printf("argc: %d\n", argc);
    char* nombre = argv[1];
    int permisos = strtol(argv[2], NULL, 8);
    printf("Cambiando permisos de '%s' a %d\n", nombre, permisos);
    if (chmod(nombre, permisos) == -1) { 
        perror("Error al cambiar permisos");
    } else {
        printf("Permisos de '%s' modificados con éxito\n", nombre);
    }
    return 0;
}