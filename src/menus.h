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

//Definimos los menus.
int num_menus = 1;
int num_lecciones;
char *m_principal[20];
char *m_lecciones[MAX_LESSONS_FOR_COURSE];

void _init_ncurses();
void _init_menus();
void _imp_menu_horizontal(t_menu menu, int seleccionat);
void _imp_menu_vertical(t_menu menu, int seleccionat);

void obten_menu_inicial();
void obten_submenu(int id_course);
void construye_menu(int id_course);

void construye_menu(int id_course){
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado

    clear();
    refresh();
    int tmp_opciones[5];
    if(id_course==0){
        obten_menu_inicial();
    }else{
        obten_submenu(id_course);
    }

    //imprimimos con la primera opción seleccionada
    muestra_titulo_menu(false);    
    descmenuwin=newwin(1, max_x, y_descmenuwin,0);  
    if(id_course==0){
        //print centered
        mvwprintw(descmenuwin, 0, ((strlen(ET_ENUNCIADO_COURSE)<max_x)?floor((max_x-strlen(ET_ENUNCIADO_COURSE))/2):0),ET_ENUNCIADO_COURSE);
        _imp_menu_vertical(menu, 1);
        tmp_opciones[0]=1;
        tmp_opciones[1]=1;
        tmp_opciones[2]=0;
        tmp_opciones[3]=0;        
        tmp_opciones[4]=0;

    }else{
        if(id_course==10 || id_course==11){//how many characters do you want?
            //print centered
            mvwprintw(descmenuwin, 0, ((strlen(ET_ENUNCIADO_RANDOM_CHARS)<max_x)?floor((max_x-strlen(ET_ENUNCIADO_RANDOM_CHARS))/2):0),ET_ENUNCIADO_RANDOM_CHARS);
        }else{
            mvwprintw(descmenuwin, 0, ((strlen(ET_ENUNCIADO_LESSON)<max_x)?floor((max_x-strlen(ET_ENUNCIADO_LESSON))/2):0),ET_ENUNCIADO_LESSON);
        }
        _imp_menu_horizontal(menu, 1);
        tmp_opciones[0]=1;
        tmp_opciones[1]=1;
        tmp_opciones[2]=0;
        tmp_opciones[3]=0;        
        tmp_opciones[4]=1;        

        
    }
    wrefresh(descmenuwin);
    
    muestra_pie(tmp_opciones);//esta línea debe ir despues del refresh();
}

int muestra_menu(int id_course){
    int seleccionat = 1;
    int eleccio = 0;
    int c;
    construye_menu(id_course);
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
                break;
            case 0x109:/*f1 introducción*/
                flag_dentro_menu_lecciones=false;
                muestra_introduccion();
                construye_menu(id_course);
                break;
            case 0x10c:/*f4 change course*/
                if(id_course!=0){
                    flag_dentro_menu_lecciones=false;
                    bucle_menus();
                    eleccio=10000;//en teoria esto no tiene que suceder nunca pero lo pongo por si acaso para que salga del bucle
                }
                break;
            default:
                refresh();
                break;
        }
        if(id_course==0){
            _imp_menu_vertical(menu, seleccionat);
        }else{
            _imp_menu_horizontal(menu, seleccionat);
        }
        if(eleccio != 0) break;
    }   
    refresh();
    delwin(descmenuwin);
    endwin();
    return eleccio-1;
}

void obten_menu_inicial(){  
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado

    int i;
    for(i=0;i<num_cursos;i++){
        m_principal[i]=obten_course_title(array_cursos[i]);
    }
    menu.menu = m_principal;
    menu.n_op = num_cursos;
    menu.startx = x_menuwin;
    menu.starty = y_menuwin;
    menu.max_x = ancho_caja_menu;
    menu.max_y = 7+(int)menu.n_op;

    menu.wmenu = newwin(menu.max_y, menu.max_x, menu.starty, menu.startx);
    keypad(menu.wmenu, TRUE);
    wrefresh(menu.wmenu);
}


void obten_submenu(int id_course){
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado

    num_lecciones=obten_num_titulos(id_course);

    for(int i=0;i<num_lecciones;i++){
        m_lecciones[i]=obten_titulo(i,id_course);
    }
    menu.menu = m_lecciones;
    menu.n_op = num_lecciones;
    menu.startx = x_menuwin;
    menu.starty = y_menuwin;
    menu.max_x = ancho_caja_menu;
    menu.max_y = ceil(num_lecciones/16)+5;
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
        if(i==3 || i==7 || i==8){
            ++y;            
        }
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
            mvwprintw(menu.wmenu, yy, xx," %.3s ",menu.menu[i]);
            wattroff(menu.wmenu, A_REVERSE);
        }
        else{
            mvwprintw(menu.wmenu, yy, xx, " %.3s ",menu.menu[i]);
        }
        xx+=4;
    }
    wrefresh(menu.wmenu);
}

void _init_ncurses(){
    /*  Initialize ncurses  */
    if((mainwin=initscr())==NULL){
        fprintf(stderr, ET_ERRORS_NCURSES1);
        exit(EXIT_FAILURE);
    }
    if(has_colors()==FALSE){
        endwin();
        printf(ET_ERRORS_NCURSES2);
        exit(EXIT_FAILURE);
    }
    if(start_color()!=OK){
        endwin();
        printf(ET_ERRORS_NCURSES3);
        exit(EXIT_FAILURE);
    }
    getmaxyx(stdscr, max_y, max_x);  
    clear();
    noecho();
    cbreak();
    curs_set(FALSE);
    refresh();
}
 
void muestra_titulo_menu(bool flag_introduccion){
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado

    char et_titulo_menu[50];
    if(!flag_introduccion){
        strcpy(et_titulo_menu,ET_PROGRAMA);
    }else{
        strcpy(et_titulo_menu,ET_INTRODUCTION_TITLE);
        strcat(et_titulo_menu," - ");
        strcat(et_titulo_menu,ET_PROGRAMA);
    }
    
    titlewin=newwin(1, max_x, y_titlewin,y_titlewin);  
    char var_barra[max_x];
    init_pair(C_TITLE,COLOR_WHITE,COLOR_MAGENTA);
    int tmp_borde=floor((max_x-strlen(et_titulo_menu))/2);

    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    wattron(titlewin, WA_BOLD | COLOR_PAIR(C_TITLE));
    mvwprintw(titlewin, 0, 0,"%s",var_barra);
    mvwprintw(titlewin, 0, tmp_borde, "%s",et_titulo_menu);
    wrefresh(titlewin);
}
