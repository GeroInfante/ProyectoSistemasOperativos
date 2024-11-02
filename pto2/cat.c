#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    char * nombre = argv[1];
    FILE *file = fopen(nombre, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    char c;
    while ((c = fgetc(file)) != EOF) {//Recorre todo el archivo de texto y lo imprime en pantalla.
        putchar(c);
    }
    putchar('\n');
    fclose(file);
    return 0;
}