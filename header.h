#ifndef FOO_DOT_H    /* This is an "include guard" */
#define FOO_DOT_H    /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // Para las constantes SIGALRM y similares
#include <unistd.h> //incluyo este para tener la funcion sleep()
#include <string.h> //incluyo para tener strcpy()

// para el lectorxml.c necesitaremos estas
#include <libxml/parser.h> 
#include <ctype.h>
#include <ncurses.h>
//

//PROTOTIPOS y VARIABLES GLOBALES main.c
int leer_tecla(int comprueba_letra, int pos_w_actual);
int num_errores;
void muestra_errores(void);
void muestra_cabecera(void);
void contar_segundos();
void pitar(void);
static int segundos=-1;
// static int j=0;
static bool var_parar_crono=true;
static int pos_w_actual=0, pos_h_actual=0;

#define C_LETRA_ERR    1
#define C_LETRA_CUR    2
#define C_TEXTO        3
#define ENTER           10
//

//PROTOTIPOS Y VARIABLES GLOBALES lectorxml.h
char **obten_todos_textos(void);
char *trim(char *s);
void obten_texto(int id_texto);
char *array_texto[15];
//
#endif /* FOO_DOT_H */
