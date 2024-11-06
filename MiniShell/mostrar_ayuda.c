#include <stdio.h>

int main()
{
    printf("Comandos disponibles:\n");
    printf("  ayuda                   - Muestra esta ayuda\n");
    printf("  mkdir <nombre>          - Crea un nuevo directorio\n");
    printf("  rmdir <nombre>          - Elimina un directorio (incluso si contiene archivos)\n");
    printf("  crear_archivo <nombre>  - Crea un archivo vacío\n");
    printf("  ls                      - Lista el contenido del directorio actual\n");
    printf("  cat <nombre>            - Muestra el contenido de un archivo\n");
    printf("  chmod <nombre> <permisos> - Cambia los permisos de un archivo: 0ABC, donde ABC es un número de 3 dígitos que representa los permisos en octal\n");
    printf("                            - A -> Permisos para el propietario del archivo\n");
    printf("                            - B -> Permisos para el grupo del archivo\n");
    printf("                            - C -> Permisos para los demás\n");
    printf("                            - Los valores pueden ser:\n");
    printf("                                1 - Ejecución\n");
    printf("                                2 - Escritura\n");
    printf("                                3 - Escritura y ejecucion\n");
    printf("                                4 - Lectura\n");
    printf("                                5 - Lectura y ejecucion\n");
    printf("                                6 - Lectura y escritura\n");
    printf("                                7 - Lectura, escritura y ejecucion\n");

    return 0;
}