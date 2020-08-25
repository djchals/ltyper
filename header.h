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

// para el lectorxml.c necesitaremos estas
#include<json-c/json.h>
#include <ctype.h> //incluyo para tener isspace()
#include <ncurses.h>


//PROTOTIPOS y VARIABLES GLOBALES main.c
int leer_tecla(int comprueba_letra);
int num_errores;
void muestra_errores(void);
void muestra_cabecera(int id_texto);
void contar_segundos();
void pitar(void);
void finalizar();
// void borrar_caja(int y, int x,int c);
void muestra_texto(int act_id_texto);


static int total_tiempo;
const static int MAX_TIEMPO=150;//El número máximo de segundos que le damos para la prueba
static int pos_w_actual, pos_h_actual;
static bool act_letra_err;//con esta controlaremos si nos hemos equivocado en la letra actual
const static int x_child_win=0, y_child_win=3;
#define POS_H_CABECERA 2
#define POS_W_CABECERA 0

static int minutos, segundos;
static int long_texto;
static int id_texto;    

int ancho_caja, alto_caja;//dimensiones de la caja

WINDOW *mainwin, *childwin;

#define C_LETRA_ERR    1
#define C_LETRA_OK     2
#define C_TIMEOUT      3
#define ENTER           10
//
//
#endif /* FOO_DOT_H */
