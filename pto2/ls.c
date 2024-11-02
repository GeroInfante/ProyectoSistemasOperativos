#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
    DIR *dir = opendir(".");
    if (!dir) {
        perror("No se pudo abrir el directorio");
        exit(1);
    }

    struct dirent *entrada;
    printf("Contenido del directorio:\n");
    while ((entrada = readdir(dir)) != NULL) {
        printf("  %s\n", entrada->d_name);
    }

    closedir(dir);
    return 0;
}