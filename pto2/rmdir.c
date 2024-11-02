#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    char* nombre = argv[1];
    DIR *dir = opendir(nombre);
    if (!dir) {
        perror("Error al abrir el directorio");
        exit(1);
    }

    struct dirent *entrada;
    char ruta[1024];

    while ((entrada = readdir(dir)) != NULL) {//Mientras hayan archivos en el directorio
        // Ignorar los directorios "." y ".."
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        snprintf(ruta, sizeof(ruta), "%s/%s", nombre, entrada->d_name);

        struct stat statbuf;
        if (stat(ruta, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {//Determina si el archivo es un directorio.
                if (fork() == 0) {
                    char* argv[] = {"rmdir", ruta, NULL};
                    execv("rmdir", argv);// Llamada recursiva para eliminar subdirectorios
                    exit(1);
                }
                wait(NULL);
            } else {
                // Eliminar archivos
                if (remove(ruta) == -1) {
                    perror("Error al eliminar archivo");
                } else {
                    printf("Archivo '%s' eliminado\n", ruta);
                }
            }
        }
    }

    closedir(dir);

    // Elimina el directorio una vez que esté vacío
    if (rmdir(nombre) == -1) {
        perror("Error al eliminar el directorio");
    } else {
        printf("Directorio '%s' eliminado con éxito\n", nombre);
    }
    return 0;
}