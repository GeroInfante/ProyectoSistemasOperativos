#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("gcc mostrar_ayuda.c -o mostrar_ayuda");
    system("gcc ls.c -o ls");
    system("gcc mkdir.c -o mkdir");  
    system("gcc crear_archivo.c -o crear_archivo");
    system("gcc rmdir.c -o rmdir");
    system("gcc cat.c -o cat");
    system("gcc chmod.c -o chmod");
    system("gcc shell.c -o main");
    printf("Se compilo la Shell con exito, ejecutela con el comando:\n ./main\n");

    return 0;
}