// Archivo donde se definen las funciones de la tabla de símbolos 
// la estructura de datos será un árbol binario de búsqueda. Este
// archivo será invocado por los analizadores léxico y sintáctico.

#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

#define NUM_PALABRAS_RESERVADAS 8

abb tabla;

void insertarElemento(char* id, int valor);
void iniciarTabla();
void liberarTabla();
void imprimirTabla();
void _impresionRecursivaArbol(abb A);

void iniciarTabla(){
    //inicializamos la estructura de datos
    crear(&tabla);

    //Array que contiene subarrays de dos elementos donde el primer 
    //elemento es un componente lexico y el segundo la palabra reservada
    
    //se insertan para que el ABB quede lo mas equilibrado posible

    char *lexemas[NUM_PALABRAS_RESERVADAS];
    lexemas[0] = "import";
    lexemas[1] = "foreach";
    lexemas[2] = "int";
    lexemas[3] = "for";
    lexemas[4] = "return";
    lexemas[5] = "double";
    lexemas[6] = "void";
    lexemas[7] = "while";

    int valores[NUM_PALABRAS_RESERVADAS];
    valores[0] = IMPORT;
    valores[1] = FOREACH;
    valores[2] = INT;
    valores[3] = FOR;
    valores[4] = RETURN;
    valores[5] = DOUBLE;
    valores[6] = VOID;
    valores[7] = WHILE;

    //insertar las palabras reservadas en la tabla de simbolos
    for(int i = 0; i < NUM_PALABRAS_RESERVADAS; i++){
        insertarElemento(lexemas[i], valores[i]);
    }
}

void insertarElemento(char* id, int valor){
    //creacion tipoelem
    tipoelem e;
    int tam = strlen(id) + 1;
    e.id = (char*)malloc(sizeof(char)*tam);
    strcpy(e.id, id);
    e.valor = valor;
    //insercion en el abb
    insertar(&tabla, e);
}

void buscarElemento(tipoelem *e){
    //comprobamos si ya esta en la tabla de simbolos
    if(es_miembro(tabla, *e)){
        tipoelem aux;
        //recuperamos su valor a partir del id
        buscar_nodo(tabla, e->id, &aux);
        e->valor = aux.valor;
    //si no esta se tratara de un identificador nuevo, se inserta
    }else{
        e->valor = ID;
        insertarElemento(e->id, e->valor);
    }
}

void liberarTabla(){
    //liberacion del espacio del  abb
    destruir(&tabla);
}

void imprimirTabla(){
    printf("************************\n");
    printf("TABLA DE SIMBOLOS\n");
    printf("************************\n");
    _impresionRecursivaArbol(tabla);
    printf("************************\n");
}

void _impresionRecursivaArbol(abb A){
    tipoelem e;
    if(!es_vacio(A)){
        _impresionRecursivaArbol(izq(A));
        leer(A,&e);
        printf("%s --> %d\n",e.id,e.valor);
        _impresionRecursivaArbol(der(A));
    }
}