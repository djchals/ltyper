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
int num_lecciones;
char *m_principal[MAX_COURSES];
char *m_lecciones[MAX_LESSONS_FOR_COURSE];

void _init_ncurses();
void _init_menus();
// void _imp_menu(t_menu menu, int seleccionat);
void _imp_menu_horizontal(t_menu menu, int seleccionat);
void _imp_menu_vertical(t_menu menu, int seleccionat);

void obten_menu_inicial();
void obten_submenu(int id_course);

int muestra_menu(int id_course){
    WINDOW *descmenuwin;
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
    muestra_titulo_menu();    
    descmenuwin=newwin(1, max_x, y_descmenuwin,x_descmenuwin);  
    if(id_course==0){
        mvwprintw(descmenuwin,0, 0, "%s",ET_ENUNCIADO_COURSE);
        _imp_menu_vertical(menu, 1);
    }else{
        mvwprintw(descmenuwin,0, 0, "%s",ET_ENUNCIADO_LESSON);
        _imp_menu_horizontal(menu, 1);
    }
    wrefresh(descmenuwin);
    
    int tmp_opciones[4]={1,0,0,0};
    muestra_pie(tmp_opciones);//esta línea debe ir despues del refresh();
    
    refresh();
    //
    while(1){
        c = wgetch(menu.wmenu);
        switch(c){
            case KEY_LEFT:
            case KEY_UP:
                if(seleccionat == 1)
                    seleccionat = menu.n_op;
                else
                    --seleccionat;
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
            case 65:
                if(seleccionat == menu.n_op)
                    seleccionat = 1;
                else
                    ++seleccionat;
                break;
            case ENTER:
                eleccio = seleccionat;
                break;
            case 27:
                //ESC exit program
                flag_dentro_menus=false;
                flag_dentro_menu_lecciones=false;
                eleccio=10000;//como le restaremos 1 se quedará en 9999
            default:
                refresh();
                break;
        }
    if(id_course==0){
        _imp_menu_vertical(menu, seleccionat);
    }else{
        _imp_menu_horizontal(menu, seleccionat);
    }

//         _imp_menu(menu, seleccionat);
        if(eleccio != 0) break;
    }   
    refresh();
    delwin(descmenuwin);
    endwin();
    return eleccio-1;
}

void obten_menu_inicial(){  
    for(int i=0;i<num_cursos;i++){
        m_principal[i]=obten_course_title(array_cursos[i]);
    }
    menu.menu = m_principal;
    menu.n_op = num_cursos;
    menu.startx = x_menuwin;
    menu.starty = y_menuwin;
    menu.max_x = ancho_caja_menu;
//     menu.max_y = alto_caja_menu;
    menu.max_y = 4+(int)menu.n_op;

    menu.wmenu = newwin(menu.max_y, menu.max_x, menu.starty, menu.startx);
    keypad(menu.wmenu, TRUE);
    wrefresh(menu.wmenu);
}


void obten_submenu(int id_course){
    num_lecciones=obten_num_titulos(id_course);

    for(int i=0;i<num_lecciones;i++){
        m_lecciones[i]=obten_titulo(i,id_course);
    }
    menu.menu = m_lecciones;
    menu.n_op = num_lecciones;
    menu.startx = x_menuwin;
    menu.starty = y_menuwin;
    menu.max_x = ancho_caja_menu;
    menu.max_y = alto_caja_submenu;
    menu.wmenu = newwin(menu.max_y, menu.max_x,  menu.starty, menu.startx);
   
    keypad(menu.wmenu, TRUE);
    wrefresh(menu.wmenu);
}

void _imp_menu_vertical(t_menu menu, int seleccionat){
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
void _imp_menu_horizontal(t_menu menu, int seleccionat){
    int x=2, y=2;//valores iniciales   
    int xx,yy;//estas dos tendrán las coordenadas de las opciones del menú colocadas en horizontal
    box(menu.wmenu, 0, 0);
    yy=y;
    xx=x;
    refresh();
    for(int i = 0; i < menu.n_op; ++i){   
        //estamos en la primera opción
        if(seleccionat==0){
        }
        //nos vamos a salir a la derecha
        if(xx>(menu.max_x-5)){
            yy++;
            xx=x;
        }
        
        if(seleccionat == i + 1){
            wattron(menu.wmenu, A_REVERSE);
//             mvwprintw(menu.wmenu, yy, xx,"yy%d xx%d menu(%s)-", yy,xx,menu.menu[i]);
            mvwprintw(menu.wmenu, yy, xx," %.3s ",menu.menu[i]);

            wattroff(menu.wmenu, A_REVERSE);
        }
        else{
//             mvwprintw(menu.wmenu, yy, xx, "yy%d xx%d menu%s- ",yy,xx, menu.menu[i]);
            mvwprintw(menu.wmenu, yy, xx, " %.3s ",menu.menu[i]);
        }
        xx+=3;
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
 
void muestra_titulo_menu(){
    wrefresh(titlewin);
    char var_barra[max_x];
    init_pair(C_TITLE,COLOR_WHITE,COLOR_MAGENTA);
    int tmp_borde=floor((max_x-strlen(ET_PROGRAMA))/2);

    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    wattron(titlewin, WA_BOLD | COLOR_PAIR(C_TITLE));
    mvwprintw(titlewin, 0, 0,"%s",var_barra);
    mvwprintw(titlewin, 0, tmp_borde, "%s",ET_PROGRAMA);
    wrefresh(titlewin);
}
