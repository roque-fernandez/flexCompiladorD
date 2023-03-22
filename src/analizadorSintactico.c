//archivo que contiene el analizador sintactico
//se encargara de consumir los componentes que el 
//analizador lexico le proporcione (de uno en uno)

#include <stdio.h>
#include <stdlib.h>
#include "analizadorLexico.h"
#include "definiciones.h"

void procesar(){
    //COMPONENTE ACTUAL
    tipoelem e;
    e.id = NULL;
    //iniciamos el componente actual con el primer componente del archivo
    siguienteComponenteLexico(&e);
    
    //mientras no se alcance el fin de fichero se procesan los componentes
    while(e.valor != EOF_COMPONENT){
    //while(count < limit){
        printf("\n<\"%s\", %d>\n",e.id, e.valor);
        //liberar identificador del componente
        if(e.id != NULL){
            free(e.id);
            e.id = NULL;
        }
        //pedir el siguiente componente al analizador lexico
        siguienteComponenteLexico(&e);
    }
    


}