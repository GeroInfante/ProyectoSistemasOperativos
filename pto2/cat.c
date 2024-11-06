#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char * nombre = argv[1];
    int file = open(nombre, O_RDONLY);
    if (file == -1) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    char c;
    while (read(file, &c, 1) > 0) {//Recorre todo el archivo de texto y lo imprime en pantalla.
        write(STDOUT_FILENO, &c, 1);
    }
    write(STDOUT_FILENO, "\n", 1);
    close(file);
    return 0;
}