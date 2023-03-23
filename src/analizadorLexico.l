/*SECCION DE LIBRERIAS*/
%{
#include "definiciones.h"
#include "tablaSimbolos.h"
#include "errores.h"
%}
%option noyywrap

/*SECCION DE EXPRESIONES REGULARES*/

/* espaciado */
DELIMITADOR    [\s\t\n\r]
ESPACIADO    {DELIM}+

/* datos basicos */
LETRA       [A-Za-z]
DIGITO      [0-9]
CERO_UNO    [01]

/* comentarios */
COMENTARIO_LINEA \/\/[^\n]*     
COMENTARIO_BLOQUE \/\*(.|\n)*\*\/
//comentarios anidados veremos mas tarde

/* ids de funciones y variables*/
ID          ({LETRA}|\_)({LETRA}|{DIGITO}|\_)*

/* numeros enteros*/
DECIMALES   {DIGITO}(\_?{DIGITO})*
BINARIOS    {BINARIO}(\_?{BINARIO})*
//NUM_DECIMAL  {DIGITO}(\_?{DECIMALES})?
NUM_BINARIO  0(b|B)\_?{BINARIOS}
ENTERO  ({DECIMALES}|{NUM_BINARIO})

/* numeros flotantes */
EXPONENTE   (e|E)(\+|\-)?{DECIMALES}
NUM_FLOTANTE1   {DECIMALES}\.{DECIMALES}?{EXPONENTE}?
NUM_FLOTANTE2   {DECIMALES}{EXP_DECIMAL}
NUM_FLOTANTE3   \.{DECIMALES}{EXP_DECIMAL}?
NUM_FLOTANTE    ({NUM_FLOTANTE1}|{NUM_FLOTANTE2}|{NUM_FLOTANTE3})

/* operadores */
OPERADOR    (\.|\,|\;|\(|\)|\{|\}|\[|\]|\>|\<|\=|\+|\-|\*|\/)
MAS_MAS     \+\+
MAS_IGUAL   \+\=
IGUAL_IGUAL     \=\=

/* strings */
STRING_COMILLA_SIMPLE   \`(.|\n)+\`
STRING_COMILLA_DOBLE    \"(.|\n)+\"
STRING  {STRING_COMILLA_SIMPLE}|{STRING_COMILLA_DOBLE}

/*  DEFINIR LO QUE SE VA A RETORNAR EN CADA CASO*/
%%
{ESPACIADO}   /* Se ignora */
{COMENTARIO_LINEA} /* Se ignora */
{COMENTARIO_BLOQUE} /* Se ignora */
{ID}        {return ID;}
{STRING}    {return STRING;}
{OPERADOR}  {return (int) *yytext;}
{MAS_MAS}   {retrurn MAS_MAS}
{MAS_IGUAL}   {retrurn MAS_IGUAL}
{IGUAL_IGUAL}   {retrurn IGUAL_IGUAL}
{ENTERO}    {return INT;}
{NUM_FLOTANTE}     {return DOUBLE;}
.           {imprimirError(4);}
<<EOF>>     {yyterminate();}
%%

void siguienteComponenteLexico(tipoelem *e){
    e->valor = yylex();
    //caso normal
    if(e->valor != 0){
        e->id = strdup(yytext);
    }
    //caso de ser un ID hay que buscar en la tabla
    if(e->valor == ID){     
        buscarElemento(e);
    }
    //caso de que se acabe el archivo se devuelve 0 pero hay que cambiar por el valor de EOF
    }else if(e->valor == 0){
        e->valor = EOF_COMPONENT;
    }
}

//abrir archivo
void abrirArchivo(char *archivoEntrada){
    FILE *archivo;
    archivo = fopen(archivoEntrada, "r");
    if(archivo == NULL){
        imprimirError(1);
    }
    yyin = archivo;
}

//cerrar archivo
void cerrarArchivo(){
    fclose(yyin);
    yylex_destroy();
}


