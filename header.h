#ifndef FOO_DOT_H    /* This is an "include guard" */
#define FOO_DOT_H    /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // Para las constantes SIGALRM y similares
#include <unistd.h> //incluyo este para tener la funcion sleep() y alarm()
#include <string.h> //incluyo para tener strcpy()
#include <math.h> //necesitaremos floor() para el cronometro

#include<json-c/json.h>
#include <ctype.h> //incluyo para tener isspace()
#include <ncurses.h>

#define MAX_COURSES 6 //debe estar aquí para que la coja en cadenas_menus.hs
#include "cadenas_menus.h"

//PROTOTIPOS y VARIABLES GLOBALES main.c
int leer_tecla(int comprueba_letra);
int num_errores;
void muestra_errores(void);
void muestra_cabecera(int id_texto, int id_course);
void muestra_pie(int opciones[3]);
void contar_segundos();
void pitar(void);
void finalizar(int id_course);
void muestra_texto(int act_id_texto, int id_course);
void actualiza_cursor(int i_row, int pos_h_actual, int pos_w_actual,unsigned char todo_texto[]);
void salir_al_menu();
bool flag_dentro_menus,flag_dentro_texto;
void bucle_menus();
void muestra_titulo_curso(int id_course);

static int total_tiempo;
const static int MAX_TIEMPO=150;//El número máximo de segundos que le damos para la prueba
static int pos_w_actual, pos_h_actual;
static bool act_letra_err;//con esta controlaremos si nos hemos equivocado en la letra actual
const static int x_child_win=0, y_child_win=4;
bool flag_timeout=false, flag_salir=false;
int ch;
#define POS_H_CABECERA 3
#define POS_W_CABECERA 0

static int minutos, segundos;//, centesimas=0;
static int long_texto;
static int id_texto;    

int ancho_caja, alto_caja;//dimensiones de la caja

WINDOW *mainwin, *childwin, *timewin, *errorwin, *finalwin, *footerwin, *titlewin;

#define C_LETRA_ERR     1
#define C_LETRA_OK      2
#define C_LETRA_PIE     3
#define C_TIMEOUT       4
#define C_TITLE         5
#define ENTER           10
//
//
#endif /* FOO_DOT_H */
