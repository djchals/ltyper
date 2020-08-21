// #include <string.h>
// #include <stdlib.h>
#define WIDTH_MENU          50
// #define HEIGHT         10
// #define /*SEP*/             2

#define MAX_MENUS      10
#define MAX_OP_MENU    10

// #define ENTER          10
//VARIABLES GLOBALES Y PROTOTIPOS
int *num_opciones;
//

int startx;
int starty;
int max_x;
int max_y;

typedef struct menus {
    WINDOW *wmenu;
    int startx;
    int starty;
    int max_x;
    int max_y; 
    char **menu;
    int n_op;
} t_menu;

t_menu menu[MAX_MENUS];

//Definim el nostres menus. Aquí un exemple.
int num_menus = 1;

void _init_ncurses();
void _init_menus();
void _imp_menus();
void _imp_menu(t_menu menu, int seleccionado);


int plantear_pregunta(int id_pregunta){
    int menu_seleccionado = 0;
    int seleccionado = 1;
    int eleccion = 0;
    int c;

    _init_ncurses();

    //Aquí se muestra la pregunta y las opciones
//     mvprintw(0, 0, );
        mvprintw(POS_H_CABECERA, POS_W_CABECERA, "%s",preguntas[id_pregunta].et_enunciado);
//     mostrar_puntuacion();
//     mvprintw(16, 0, "Escojo: ");


    _init_menus(id_pregunta);
    _imp_menus();
   
    //LA PRIMERA VEZ LO IMPRIMIMOS PARA QUE SALGA LA PRIMERA OPCIÓN SELECCIONADA
    _imp_menu(menu[menu_seleccionado], seleccionado);

    
    while(1){
        c = wgetch(menu[menu_seleccionado].wmenu);
        switch(c){
                case KEY_UP:
                if(seleccionado == 1)
                    seleccionado = menu[menu_seleccionado].n_op;
                else
                    --seleccionado;
                break;
            case KEY_DOWN:
                if(seleccionado == menu[menu_seleccionado].n_op)
                    seleccionado = 1;
                else
                    ++seleccionado;
                break;
            case KEY_LEFT:
                if (( seleccionado == 1 ) && ( menu_seleccionado > 0 )) menu_seleccionado--;
                break;
            case KEY_RIGHT:
                if ( ( seleccionado == 1 ) && ( menu_seleccionado < num_menus - 1 )) menu_seleccionado++;
                break;
            case ENTER:
                eleccion = seleccionado;
                break;
            default:
                refresh();
                break;
        }
        _imp_menu(menu[menu_seleccionado], seleccionado);
        if(eleccion != 0) break;
    }   
    mvprintw(14, 0, "Escojo: %s\n",menu[menu_seleccionado].menu[eleccion - 1]);refresh();getchar();
    clrtoeol();
    refresh();
    endwin();
    
    return eleccion;
}

void _init_menus(int id_pregunta){
    //definimos el array literal que luego rellenaremos    
    char **array_opciones = malloc(10 * sizeof(char));
    for(int i=0;i<preguntas[id_pregunta].num_opciones;i++){
        array_opciones[i]=preguntas[id_pregunta].et_respuesta[i];
    }
    //construimos el menu    
    menu[0].menu = array_opciones;
    menu[0].n_op=preguntas[id_pregunta].num_opciones;
    menu[0].startx = 0;
    menu[0].starty = 3;
    menu[0].max_x = WIDTH_MENU;
    menu[0].max_y = preguntas[id_pregunta].num_opciones+4;//Dejaremos dos líneas por abajo y por arriba de margen
    menu[0].wmenu = newwin(menu[0].max_y, menu[0].max_x, menu[0].starty, menu[0].startx);
    keypad(menu[0].wmenu, TRUE);

    refresh();
}

void _imp_menus(){
int i;
    for ( i = 0; i < num_menus; i ++ )    _imp_menu(menu[i], 0);
}

void _imp_menu(t_menu menu, int seleccionado){
    int x, y, i;   

    x = 2;
    y = 2;
    box(menu.wmenu, 0, 0);
    for(i = 0; i < menu.n_op; ++i){   
        if(seleccionado == i + 1){
            wattron(menu.wmenu, A_REVERSE);
            mvwprintw(menu.wmenu, y, x, "%d) %s", i+1, menu.menu[i]);
            wattroff(menu.wmenu, A_REVERSE);
        }
        else
            mvwprintw(menu.wmenu, y, x, "%d) %s", i+1, menu.menu[i]);
        ++y;
    }
    wrefresh(menu.wmenu);
}

void _init_ncurses(){
   initscr();
   getmaxyx(stdscr, max_y, max_x);  
   clear();
   noecho();
   cbreak();
   curs_set(FALSE);
   refresh();
 }
