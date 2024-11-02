#include <stdio.h>
#include <sys/stat.h>

int main(int argc,char* argv[])
{
    char* nombre = argv[1];
    if (mkdir(nombre, 0777) == -1) {  //Al crear el directorio se setean todos los permiso de lectura escritura y ejecucion
        perror("Error al crear el directorio");
    } else {
        printf("Directorio '%s' creado con éxito\n", nombre);
    }
    return 0;
}