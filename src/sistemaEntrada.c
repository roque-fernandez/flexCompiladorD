#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "definiciones.h"
#include "errores.h"

#define TAM 128

//puntero al archivo donde esta el codigo fuente
FILE *archivo;
//buffers
char A[TAM+1],B[TAM+1];
//punteros para implementar doble centinela
char *inicio, *fin;

//Codigos: A->0, B->1
void cargarBuffer(int codigoBuffer);
void imprimirBuffer(int codigoBuffer);
void avanzar();
void retroceder();


void iniciarSistemaEntrada(char *archivoEntrada){
    //abrir archivo
    archivo = fopen(archivoEntrada, "r");
    if(archivo == NULL){
        imprimirError(1);
    }
    //inicializar buffers
    A[TAM] = EOF;
    B[TAM] = EOF;
    //inicializar punteros
    inicio = A;
    fin = A;
    //cargar datos en el primer buffer
    cargarBuffer(0);
    //imprimirBuffer(0);
}

//devuelve al analizador lexico el caracter correspondiente
char siguienteCaracter(){
    if(*fin == EOF){
        //comprobar que no estemos al final de un buffer
        //final buffer A
        if(fin == A+TAM){
            //comprobar si inicio esta en el buffer que se va a cargar
            if(inicio > &B[0] && inicio < &B[TAM]){
                imprimirError(3);
                return EOF;
            }
            cargarBuffer(1);
        }
        //final buffer B
        else if(fin == B+TAM){
            //comprobar si inicio esta en el buffer que se va a cargar
            if(inicio > &A[0] && inicio < &A[TAM]){
                imprimirError(3);
                return EOF;
            }
            cargarBuffer(0);
        }
        else{
            fin++;
            return EOF;
        } 
    }
    //caso normal
    char c = *fin;
    fin++;
    return c;
    
}

//Codigos: A->0, B->1
void cargarBuffer(int codigoBuffer){
    //variable por si los caracteres leidos no ocupan todo el buffer 
    int nCaracteresLeidos;
    //buffer A
    if(codigoBuffer == 0){
        nCaracteresLeidos = fread(A,sizeof(char),TAM,archivo);
        fin = A;
        //si no se ocupa todo el buffer se adelanta el EOF
        //solo puede ocurrir en la ultima carga
        if (nCaracteresLeidos < TAM){
            A[nCaracteresLeidos] = EOF;
        }
    }
    else if(codigoBuffer == 1){
        nCaracteresLeidos = fread(B,sizeof(char),TAM,archivo);
        fin = B;
        if (nCaracteresLeidos < TAM){
            B[nCaracteresLeidos] = EOF;
        }
    }
}

//funcion que devuelve al analizador lexico el lexema que se
//encuentra entre los punteros de inicio y fin
void devolverLexema(tipoelem *e){
    int indInicio,indFin,tamLexema;
    int indLexema = 0;
    //lo primero es calcular los indices donde estan inicio y fin
    
    //dependiendo de en que buffer este se calculara un indice u otro
    if(inicio >= &A[0] && inicio <= &A[TAM] ){
        indInicio = inicio -&A[0];
    }else{
        indInicio = inicio - &B[0] + TAM; //si esta en el bloque B se suman las posiciones del A
    }
    
    //dependiendo de en que buffer este se calculara un indice u otro
    if(fin >= &A[0] && fin <= &A[TAM] ){
        indFin = fin - &A[0];
    }else{
        indFin = fin  - &B[0] + TAM; //si esta en el bloque B se suman las posiciones del A
    }
    
    //calculamos el tamaño del lexema dependera del orden de los indices
    //hay que tener en cuenta el \0 del final
    if(indInicio < indFin){
        tamLexema = indFin - indInicio + 1;
    }
    else{
        tamLexema = ((2*TAM - indInicio) + indFin) + 1;
    }
    
    //comprobamos que no sobrepase el tamaño maximo que especificamos
    if(tamLexema > TAM){
        imprimirError(2);
    }
    
    e->id = (char*)malloc(sizeof(char)*tamLexema);
    //ir leyendo las posiciones y añadiendolas al identificador del componente
    while(inicio != fin){
        e->id[indLexema] = *inicio;
        avanzar();
        indLexema++;
    }
    e->id[indLexema] = '\0';
    
}

//avanzar inicio
void avanzar(){
    inicio++;
    //si al avanzar nos pasamos del tamaño del buffer
    //pasamos al siguiente
    if(inicio == A+TAM){
        inicio = B;
    }else if(inicio == B+TAM){
        inicio = A;
    }
}

//devolver caracter
void retroceder(){
    //caso de que este al principio de un buffer
    if(fin == A){
        fin = B+TAM;
    }
    else if(fin == B){
        fin = A+TAM;
    }
    //caso general
    else{
        fin--;
    }
}

//liberar punteros y cerrar archivo
void liberarSistemaEntrada(){
    inicio=NULL;
    fin=NULL;
    fclose(archivo);
}

void imprimirBuffer(int codigoBuffer){
    printf("****************************\n");
    if(codigoBuffer == 0){
        printf("Buffer A: %s\n",A);
    }
    else if(codigoBuffer == 1){
        printf("Buffer B: %s\n",B);
    }
}

void omitirComentario(){
    //se comprueba el puntero de fin para que no se quede al finall de un buffer
    if (fin == A+TAM){
        cargarBuffer(1);
    }else if(fin == B+TAM){
        cargarBuffer(0);
    }
    //se actualiza el puntero de inicio para que cuando
    //se devuelva el siguiente lexema no se tenga en cuenta el comentario
    inicio = fin;
}

