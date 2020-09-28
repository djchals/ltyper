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
// #include <ncursesw/curses.h>
#include <curses.h>

#define MAX_COURSES 6 //debe estar aquí para que la coja en cadenas_menus.hs
#define MAX_LESSONS_FOR_COURSE 40
#include "strings.h"

#include <locale.h>

//PROTOTIPOS y VARIABLES GLOBALES main.c
int leer_tecla(int comprueba_letra);
int num_errores;
void muestra_errores(void);
void muestra_cabecera(int id_texto, int id_course);
void muestra_pie(int opciones[4]);
void contar_segundos();
void pitar(void);
void finalizar(int id_course,bool flag_cancela_texto);
void muestra_texto(int act_id_texto, int id_course);
void actualiza_cursor(int i_row, int pos_h_actual, int pos_w_actual, int flag_attrs,unsigned char todo_texto[]);
void seleccionar_menu();
bool flag_dentro_menus,flag_dentro_menu_lecciones,flag_dentro_texto;
void bucle_menus();
void muestra_titulo_curso(int id_course);
void muestra_titulo_menu(bool flag_introduccion);
int comprueba_tecla(int ch, int i_row, unsigned char todo_texto[]);
bool is_special(int tmp_caracter);
int contar_195(int i_row, unsigned char todo_texto[]);

//introduction.h
void muestra_introduccion();
void muestra_presentacion();
void dibuja_manos();
void dibuja_presentacion();

bool flag_primera_vez=true;//solo mostraremos la intro la primera vez

//menus.h
#define ENTER          10

int startx;
int starty;
int max_x;
int max_y;
void _init_ncurses();
//

static int total_tiempo;
const static int MAX_TIEMPO=150;//El número máximo de segundos que le damos para la prueba
static int pos_w_actual, pos_h_actual;
static bool act_letra_err;//con esta controlaremos si nos hemos equivocado en la letra actual
void obten_coord_wins();

bool flag_muestra_borde_keyb=true;
static int x_childwin, y_childwin;
static int x_timewin, y_timewin;
static int x_errorwin, y_errorwin;
static int x_finalwin, y_finalwin;
static int x_footerwin, y_footerwin;
static int x_keyboardwin, y_keyboardwin;
static int x_keybintrowin, y_keybintrowin;
static int x_titlewin, y_titlewin;
static int x_lessonwin, y_lessonwin;
static int x_descwin, y_descwin;
static int x_menuwin, y_menuwin;
static int x_descmenuwin, y_descmenuwin;
static int x_descfinalwin, y_descfinalwin;
static int x_frasefinalwin, y_frasefinalwin;
static int x_introductionwin,y_introductionwin;
static int x_prwin,y_prwin;
bool flag_timeout=false, flag_salir=false;
int ch;

static int minutos, segundos;//, centesimas=0;
static int long_texto;
static int id_texto;    

//inicializamos las variables para la childwin y el cursor
int ancho_caja=80, alto_caja=10;//dimensiones de la caja
int ancho_caja_menu=70, alto_caja_menu=10,alto_caja_submenu=6;
int ancho_caja_final=50, alto_caja_final=8;

WINDOW *mainwin, *childwin, *timewin, *errorwin, *keyboardwin, *titlewin, *lessonwin, *descwin, *handswin, *prwin,*descmenuwin;

#define C_LETRA_ERR     1
#define C_LETRA_OK      2
#define C_LETRA_PIE     3
#define C_TIMEOUT       4
#define C_TITLE         5
#define C_TITLE         5
#define ENTER           10
#define C_LETRA_PRUEBA  20
#define KEYB_WHITE      6
#define KEYB_BLACK      7
#define KEYB_FINGER1     8
#define KEYB_FINGER2     9
#define KEYB_FINGER3     10
#define KEYB_FINGER4     11
#define KEYB_FINGER5     12    
#define KEYB_FINGER6     13   

#define MAX_Y_INTRO 20
#define MAX_X_INTRO 47

#define DATADIR "/usr/local/share/ltyper/db/"
//
//
#endif /* FOO_DOT_H */
