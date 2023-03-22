#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROJO_T "\x1b[31m"

void imprimirError(int codigo){
    switch(codigo){
        case 1:
            printf(ROJO_T "\nError: Fallo en la apertura de archivo\n");
            exit(EXIT_FAILURE);
            break;
        case 2:
            printf(ROJO_T "\nError: La longitud del lexema excede el límite permitido\n");
            break;
        case 3:
            printf(ROJO_T "\nError: El bloque no se puede cargar ya que se perdería puntero a inicio\n");
            exit(EXIT_FAILURE);
            break;
        case 4:
            printf(ROJO_T "\nError: Caracter desconocido\n");
            exit(EXIT_FAILURE);
            break;
        case 5:
            printf(ROJO_T "\nError: Formato de exponente incorrecto\n");
            break;
        default:
            printf(ROJO_T "\nError: Desconocido\n");
            break;
    }
}