#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

// Cambiar permisos de un archivo
void cambiar_permisos(const char *nombre, int permisos) {
    if (chmod(nombre, permisos) == -1) { 
        perror("Error al cambiar permisos");
    } else {
        printf("Permisos de '%s' modificados con éxito\n", nombre);
    }
}

// Función principal del mini shell
int main() {
    char comando[256];

    printf("Bienvenido al Mini Shell. Escriba 'ayuda' para ver los comandos disponibles.\n");

    while (1) {
        printf("> ");
        scanf("%s", comando);

        if (strcmp(comando, "ayuda") == 0) {
            if(fork() == 0){
                execv("mostrar_ayuda", NULL);
                exit(1);
            }
        } else
        if (strcmp(comando, "mkdir") == 0) {//No acepta nombres extremadamente largos.
            char nombre[256];
            scanf("%s", nombre);
            char* argv[] = {"mkdir", nombre, NULL};
            if(fork() == 0){
                execv("mkdir", argv);
                exit(1);
            }
        } else
        if (strcmp(comando, "rmdir") == 0) {
            char nombre[256];
            scanf("%s", nombre);
            char* argv[] = {"rmdir", nombre, NULL};
            if(fork() == 0){
                execv("rmdir", argv);
                exit(1);
            }
        } else
        if (strcmp(comando, "crear_archivo") == 0) {
            char nombre[256];
            scanf("%s", nombre);
            char* argv[]={"crear_archivo", nombre, NULL};
            if(fork() == 0){
                execv("crear_archivo", argv);
                exit(1);
            }
        } else
        if (strcmp(comando, "ls") == 0) {
            if(fork() == 0){
                execv("ls", NULL);
                exit(1);
            }
        } else
        if (strcmp(comando, "cat") == 0) {
            char nombre[256];
            scanf("%s", nombre);
            char* argv[] = {"cat", nombre, NULL};            
            if(fork() == 0){
                execv("cat", argv);
                exit(1);
            }
        } else
        if (strcmp(comando, "chmod") == 0) {
            char nombre[256];
            int permisos;
            scanf("%s %o", nombre, &permisos);
            char* argv[] = {"chmod", nombre, (char*)permisos,NULL};
            if(fork() == 0){
                execv("chmod", argv);
                exit(1);
            }
        } else {
            printf("Comando no reconocido. Escriba 'ayuda' para ver los comandos disponibles.\n");
        }
        wait(NULL);
    }

    return 0;
}
