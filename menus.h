#define WIDTH_MENU          70
#define HEIGHT_MENU         10
#define SEP             2

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

t_menu menu;

//Deinimos los menus.
int num_menus = 1;
char *m_principal[]={};
char *m_lecciones[] = {};

int num_lecciones;

void _init_ncurses();
void _init_menus();
void _imp_menu(t_menu menu, int seleccionat);
void obten_menu_inicial();
void obten_submenu(int id_course);

int muestra_menu(int id_course){
int seleccionat = 1;
int eleccio = 0;
int c;

    _init_ncurses();
    if(id_course==0){
        obten_menu_inicial();
    }else{
        obten_submenu(id_course);
    }

    //imprimimos con la primera opción seleccionada
    mvprintw(2, 0, "%s",ET_ENUNCIADO_MENU);
    _imp_menu(menu, 1);
    refresh();
    //
    while(1){
        c = wgetch(menu.wmenu);
        switch(c){
                case KEY_UP:
                if(seleccionat == 1)
                    seleccionat = menu.n_op;
                else
                    --seleccionat;
                break;
            case KEY_DOWN:
                if(seleccionat == menu.n_op)
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
        _imp_menu(menu, seleccionat);
        if(eleccio != 0) break;
    }   
    refresh();
    endwin();
    return eleccio-1;
}

void obten_menu_inicial(){  
    for(int i=0;i<num_cursos;i++){
        m_principal[i]=obten_course_title(array_cursos[i]);
    }
    menu.menu = m_principal;
    menu.n_op = num_cursos;
    menu.startx = 0;
    menu.starty = 3;
    menu.max_x = WIDTH_MENU;
    menu.max_y = HEIGHT_MENU;
    menu.wmenu = newwin(menu.max_y, menu.max_x, menu.starty, menu.startx);
    keypad(menu.wmenu, TRUE);
    refresh();
}


void obten_submenu(int id_course){
    num_lecciones=obten_num_titulos(id_course);
    for(int i=0;i<num_lecciones;i++){
        m_lecciones[i]=obten_titulo(i,id_course);
    }
    menu.menu = m_lecciones;
    menu.n_op = num_lecciones;
    menu.startx = 0;
    menu.starty = 3;
    menu.max_x = WIDTH_MENU;
    menu.max_y = HEIGHT_MENU;
    menu.wmenu = newwin(menu.max_y, menu.max_x, menu.starty, menu.startx);
    keypad(menu.wmenu, TRUE);
    refresh();
}

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
    /*  Initialize ncurses  */

    if((mainwin=initscr())==NULL){
        fprintf(stderr, "Error initializing ncurses.\n");
        exit(EXIT_FAILURE);
    }
    if(has_colors()==FALSE){
        endwin();
        printf("Your terminal does not support color\n");
        exit(EXIT_FAILURE);
    }
    if(start_color()!=OK){
        endwin();
        printf("Your terminal cannot start colors\n");
        exit(EXIT_FAILURE);
    }
    
   getmaxyx(stdscr, max_y, max_x);  
   clear();
   noecho();
   cbreak();
   curs_set(FALSE);
   refresh();
 }
