#define ET_ENUNCIADO_MENU "Mecacurses - Elige una opción"

#define WIDTH_MENU          70
#define HEIGHT_MENU         10
#define SEP             2

#define MAX_MENUS      10
#define MAX_OP_MENU    100

#define ENTER          10

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
char *m_principal[] = {
    "Curso de mecanografia",
    "Practicar con texto personalizado",
    "Configurar",
    "Salir"
};

char *m_lecciones[] = {};
int num_lecciones;

void _init_ncurses();
void _init_menus();
// void _imp_menus();
void _imp_menu(t_menu menu, int seleccionat);

int muestra_menu(int menu_seleccionat){
int seleccionat = 1;
int eleccio = 0;
int c;

    _init_ncurses();
    _init_menus();
//     _imp_menus();

    //imprimimos con la primera opción seleccionada
    mvprintw(2, 0, "%s",ET_ENUNCIADO_MENU);
    _imp_menu(menu[menu_seleccionat], 1);
    refresh();
    //
    while(1){
        c = wgetch(menu[menu_seleccionat].wmenu);
        switch(c){
                case KEY_UP:
                if(seleccionat == 1)
                    seleccionat = menu[menu_seleccionat].n_op;
                else
                    --seleccionat;
                break;
            case KEY_DOWN:
                if(seleccionat == menu[menu_seleccionat].n_op)
                    seleccionat = 1;
                else
                    ++seleccionat;
                break;
            case ENTER:
                eleccio = seleccionat;
                break;
            default:
                refresh();
                break;
        }
//             mvprintw(15, 0, "Menu seleccionat: %d", menu_seleccionat);refresh();
        _imp_menu(menu[menu_seleccionat], seleccionat);
        if(eleccio != 0) break;
    }   
//     mvprintw(16, 0, "Has escollit %d amb l'elecció %s\n", eleccio, menu[menu_seleccionat].menu[eleccio - 1]);refresh();getchar();
    clrtoeol();
    refresh();
    endwin();
    return eleccio-1;
}

void _init_menus(){
//Init menu grups    

menu[0].menu = m_principal;
menu[0].n_op = sizeof(m_principal) / sizeof(char *);
menu[0].startx = 0;
menu[0].starty = 3;
menu[0].max_x = WIDTH_MENU;
menu[0].max_y = HEIGHT_MENU;
menu[0].wmenu = newwin(menu[0].max_y, menu[0].max_x, menu[0].starty, menu[0].startx);
keypad(menu[0].wmenu, TRUE);


num_lecciones=obten_num_titulos();
for(int i=0;i<num_lecciones;i++){
    m_lecciones[i]=obten_titulo(i);
}

menu[1].menu = m_lecciones;
menu[1].n_op = num_lecciones;
menu[1].startx = 0;
menu[1].starty = 3;
menu[1].max_x = WIDTH_MENU;
menu[1].max_y = HEIGHT_MENU;
menu[1].wmenu = newwin(menu[1].max_y, menu[1].max_x, menu[1].starty, menu[1].startx);
keypad(menu[1].wmenu, TRUE);

refresh();
}

// void _imp_menus(){
// int i;
//     for ( i = 0; i < num_menus; i ++ )    _imp_menu(menu[i], 0);
// }

void _imp_menu(t_menu menu, int seleccionat){
    int x, y, i;   

    x = 2;
    y = 2;
    box(menu.wmenu, 0, 0);
    for(i = 0; i < menu.n_op; ++i){   
        if(seleccionat == i + 1){
            wattron(menu.wmenu, A_REVERSE);
            mvwprintw(menu.wmenu, y, x, "%s", menu.menu[i]);
            wattroff(menu.wmenu, A_REVERSE);
        }
        else
            mvwprintw(menu.wmenu, y, x, "%s", menu.menu[i]);
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
