#ifndef DEF_H
#define DEF_H

//COMPONENTES LEXICOS
#define IMPORT 300
#define DOUBLE 301
#define INT 302
#define WHILE 303
#define FOR 304
#define FOREACH 305
#define RETURN 306
#define VOID 307
#define IGUAL_IGUAL 308
#define MAS_IGUAL 309
#define MAS_MAS 310
#define BINARIO 311
#define STRING 320

#define ID 350
#define EOF_COMPONENT 400

// TIPOELEM (lexema, valor del componente lexico)
typedef struct  {
    char *id;
    int valor;
}tipoelem;

#endif