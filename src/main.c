#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablaSimbolos.h"
#include "analizadorSintactico.h"
#include "lex.yy.h"


int main(){

    abrirArchivo("regression.d");

    //inicializar tabla de simbolos y mostrarla por pantalla
    iniciarTabla();
    imprimirTabla();

    //iniciar el procesamiento del analizador sintactico
    procesar();

    //liberacion de memoria de la tabla y del sistema de entrada
    liberarTabla();
    cerrarArchivo();
    
}
