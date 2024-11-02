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
    printf("  chmod <nombre> <permisos> - Cambia los permisos de un archivo:\n Permiso 0444 solo lectura\n Permiso 0644 solo lectura para otro usuario. Escritura, lectura y ejecucion para el usuario \n Permiso 0755 ejecucion y lectura para otros usuarios. Escritua, lectura y ejecucion para el usuario\n");
    return 0;
}