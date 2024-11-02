#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, char* argv[])
{
    char* nombre = argv[1];
    mode_t permisos = strtol(argv[2], NULL, 8);
    if (chmod(nombre, permisos) == -1) { 
        perror("Error al cambiar permisos");
    } else {
        printf("Permisos de '%s' modificados con éxito\n", nombre);
    }
    return 0;
}