#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "definiciones.h"
#include "tablaSimbolos.h"
#include "sistemaEntrada.h"
#include "errores.h"

//AUTOMATAS
void automataAlfanumerico(tipoelem *e);
void automataNumeros(tipoelem *compActual);
void automataOperadores(tipoelem *e);
void automataStrings(tipoelem *e);
int automataComentarios(tipoelem *e);


//caracter que se va a procesar
char carActual = 0;

// [1]Caracter o barra baja [2]Numero [3]Espaciado [4]EOF [5]Operadores [6]String [7]Comentarios
int tipoCaracterInicial(char c){
    if( isalpha(c) || c == '_'){
        return 1;
    }
    else if( isdigit(c)){
        return 2;
    }
    else if(c == ' ' || c == '\n' || c == '\t' || c == '\r'){
        return 3;
    }
    else if(c == EOF){
        return 4;
    }
    else if(c == '.' || c == ',' || c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '<' || c == '>' || c == '=' || c == '+' || c == '-' || c == '*'){
        return 5;
    }
    else if(c == '\'' || c == '\"'){
        return 6;
    }
    else if(c == '/'){
        return 7;
    }
    else{
        return 0;
    }
}


void siguienteComponenteLexico(tipoelem *e){
    int flagComponenteValido = 0;
    //caracter que se va a procesar
    carActual = siguienteCaracter();
    //variable que guarda el tipo de caracter
    int tipo = tipoCaracterInicial(carActual);

    while(!flagComponenteValido){
        
        //switch para decidir a que automata debe ir el componente lexico
        switch(tipo){
            case 1:
                //ALFANUMERICO
                automataAlfanumerico(e);
                flagComponenteValido = 1;
                break;
            case 2:
                //NUMEROS
                automataNumeros(e);
                flagComponenteValido = 1;
                break;
            case 3:
                //ESPACIADO
                //se avanza el inicio ya que no se pasa el lexema al sintactico
                carActual = siguienteCaracter();
                tipo = tipoCaracterInicial(carActual);
                avanzar();
                break;
            case 4:
                //EOF
                e->valor = EOF_COMPONENT;
                flagComponenteValido = 1;
                break;
            case 5:
                //OPERADORES
                automataOperadores(e);
                flagComponenteValido = 1;
                break;
            case 6:
                //STRINGS
                automataStrings(e);
                flagComponenteValido = 1;
                break;
            case 7:
                //COMENTARIOS
                //caso de que se detecte una / que sea un operador y no un comentario
                if(automataComentarios(e)){
                    tipo = 5;
                }
                else{
                    carActual = siguienteCaracter();
                    tipo = tipoCaracterInicial(carActual);
                }
                                
                break;
            default:
                imprimirError(4);
        }
    }
    
        
    
}

//AUTOMATAS

void automataAlfanumerico(tipoelem *e){
    //aceptamos como validos numeros letras y _
    while( isalnum(carActual) || carActual == '_'){
        carActual = siguienteCaracter();
    }
    //retrocedemos en el SE ya que el lexema acaba en el siguiente caracter
    retroceder();
    //obtener el lexema
    devolverLexema(e);
    //buscarlo en la tabla de simbolos
    buscarElemento(e);   
}

void automataNumeros(tipoelem *e){
    //[0]Int normal [1]Double [2]Int Binario [3]Double con exponente 
    int tipoNumeros = 0;
    int fin = 0;
    
    //determinar si es binario
    if(carActual == '0'){
        //flagZero = 1;
        carActual = siguienteCaracter();
        if(carActual == 'b' || carActual == 'B'){
            tipoNumeros = 2;
        }
        else{
            retroceder();
        }
    }

    while(fin == 0){
        carActual = siguienteCaracter();
        
        //BINARIOS
        if(tipoNumeros == 2){
            if(carActual != '0' && carActual != '1' && carActual != '_'){
                fin = 1;
            }
        }
        //ENTEROS
        else if(tipoNumeros == 0){
            if(carActual == '.'){
                tipoNumeros = 1;               
            }
            else if(carActual == 'e' || carActual == 'E'){
                tipoNumeros = 1;
                retroceder();
            }
            else if(carActual == '_'){
                
            }
            else if(!isdigit(carActual)){
                fin = 1;
            }
        }
        //FLOTANTES
        else if(tipoNumeros == 1){
            //Solo los flotantes pueden llevar exponente
            if(carActual == 'e' || carActual == 'E'){
                carActual = siguienteCaracter();
                if(isdigit(carActual) || carActual == '+' || carActual == '-'){
                    tipoNumeros = 3;
                }
                else{
                    imprimirError(5);
                }
            }
            else if(carActual == '_'){
                
            }
            else if(!isdigit(carActual)){
                fin = 1;
            } 
        }
        //si ya ha salido el exponente solo pueden llegar numeros despues
        else if(tipoNumeros == 3){
            if(carActual == '_'){
                
            }
            else if(!isdigit(carActual)){
                fin = 1;
            }
        }
    }
    //retrocedemos en el SE ya que el num acaba en el siguiente caracter
    retroceder();
    //obtener el lexema
    devolverLexema(e);
    
    //asignar el tipo numero
    if(tipoNumeros == 0 || tipoNumeros == 2){
        e->valor = INT;
    }
    else if(tipoNumeros == 1 || tipoNumeros == 3){
        e->valor = DOUBLE;
    }
    
}

void automataOperadores(tipoelem *e){
    //operadores que tener dos caracteres
    //caso es += o ++
    if(carActual == '+'){
        carActual = siguienteCaracter();
        if(carActual == '+'){ 
            e->valor = MAS_MAS;
        }  
        else if(carActual == '='){
            e->valor = MAS_IGUAL;
        }
        else{
            retroceder();
            //asignamos el valor ascii del caracter
            e->valor = (int) carActual;
        }
    }
    //caso es ==
    else if(carActual == '='){
        carActual = siguienteCaracter();
        if(carActual == '='){
            e->valor = IGUAL_IGUAL;
        }   
        else{
            retroceder();
            //asignamos el valor ascii del caracter
            e->valor = (int) carActual;
        }
    }
    //caso normal
    else{
        //asignamos el valor ascii del caracter
        e->valor = (int) carActual;
    }
    
    //obtener el lexema
    devolverLexema(e);    
}

void automataStrings(tipoelem *e){
    //se usa para saber donde termina el string
    char tipoComillas = carActual;
    int fin = 0;
    int flagIgnorar = 0;
    
    while(fin == 0){
        carActual = siguienteCaracter();
        // el caracter \ hace que el siguiente caracter se ignore
        if(carActual == '\\' && flagIgnorar == 0){
            flagIgnorar = 1;
        }
        //caso de que se llega a las comillas finales
        else if(flagIgnorar == 0 && carActual == tipoComillas){
            devolverLexema(e);
            e->valor = STRING;
            fin = 1;
        }
        //caso de caracter normal
        else{
            flagIgnorar = 0;
        }
    }
}

//devuelve un 1 si el / es un operador y no el principio de un comentario
int automataComentarios(tipoelem *e){
    carActual = siguienteCaracter(); 
    //comentario de una linea
    if(carActual == '/'){
        //Se lee hasta llegar a nueva linea
        while(carActual != '\n'){
            carActual = siguienteCaracter();
        }
        //retroceder();
        omitirComentario();
        return 0;
    }
    //comentario de varias lineas
    else if(carActual == '*'){
        int finComentario = 0;
        while(finComentario == 0){
            carActual = siguienteCaracter();
            if(carActual == '*'){
                carActual = siguienteCaracter();
                if(carActual == '/'){
                    finComentario = 1;
                }
                else{
                    retroceder();
                }
            }
        }
        omitirComentario();
        return 0;
    }
    //comentario anidado
    else if(carActual == '+'){
        //contador de cuantos comentarios hay abiertos en un cierto momento
        int contador = 1;
        while(contador != 0){
            carActual = siguienteCaracter();
            //caso de que se abre otro comentario dentro
            if(carActual == '/'){
                carActual = siguienteCaracter();
                if(carActual == '+'){
                    contador++;
                }
                else{
                    retroceder();
                }
            }
            //caso de que se cierra un comentario
            if(carActual == '+'){
                carActual = siguienteCaracter();
                if(carActual == '/'){
                    contador--;
                }
                else{
                    retroceder();
                }
            }
        }
        omitirComentario();
        return 0;
    }
    //no es un comentario
    else{
        return 1;
    }
            

}
