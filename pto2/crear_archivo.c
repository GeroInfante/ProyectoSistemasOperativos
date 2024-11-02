#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char* nombre = argv[1];
    int fd = open(nombre, O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("Error al crear el archivo");
    } else {
        printf("Archivo '%s' creado con éxito\n", nombre);
        close(fd);
    }
    return 0;
}