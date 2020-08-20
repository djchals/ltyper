#ifndef FOO_DOT_H    /* This is an "include guard" */
#define FOO_DOT_H    /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // Para las constantes SIGALRM y similares
#include <unistd.h> //incluyo este para tener la funcion sleep()
#include <string.h> //incluyo para tener strcpy()
#include <math.h> //necesitaremos floor() para el cronometro


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
void finalizar();

static int total_tiempo=-1;
const static int MAX_TIEMPO=150;//El número máximo de segundos que le damos para la prueba
// static bool var_parar_crono=true;
static int pos_w_actual=0, pos_h_actual=0;
static bool act_letra_err=false;//con esta controlaremos si nos hemos equivocado en la letra actual
const static int x_child_win=0, y_child_win=3;
    
WINDOW *mainwin, *childwin;

#define C_LETRA_ERR    1
#define C_LETRA_OK     2
#define C_TIMEOUT      3
#define ENTER           10
//

//PROTOTIPOS Y VARIABLES GLOBALES lectorxml.h
// char **obten_todos_textos(void);
char *trim(char *s);
char *obten_texto(int id_texto);
//
#endif /* FOO_DOT_H */
