#ifndef SISTEMAENTRADA_H
#define SISTEMAENTRADA_H


void iniciarSistemaEntrada(char* archivoEntrada);

char siguienteCaracter();

void devolverLexema(tipoelem *e);

void avanzar();

void retroceder();

void liberarSistemaEntrada();

void omitirComentario();

#endif