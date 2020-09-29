void muestra_presentacion(){
    muestra_titulo_menu(false);
    dibuja_presentacion();
    sleep(1.5);
}
void muestra_introduccion(){
    int opciones_pie[5]={2,0,0,0,0};
    int ch;
    muestra_titulo_menu(true);
    
    WINDOW *introductionwin;
    introductionwin=newwin(max_y, max_x, y_introductionwin,x_introductionwin); 
    
    wprintw(introductionwin,"%s\n\n",ET_INTRODUCTION_1);
    wprintw(introductionwin,"%s\n\n",ET_INTRODUCTION_2);
    wprintw(introductionwin,"%s\n\n",ET_INTRODUCTION_3);
    wprintw(introductionwin,"%s\n\n",ET_INTRODUCTION_4);
    wprintw(introductionwin,"%s",ET_INTRODUCTION_5);
    wrefresh(introductionwin);
    muestra_pie(opciones_pie);
    ch=getch();
    switch(ch){
        case 27:
            //ESC exit program
            flag_dentro_menus=false;
            flag_dentro_menu_lecciones=false;
            return;
            break;
    }
    wclear(introductionwin);
    mvwprintw(introductionwin,0,1,"%s\n\n",ET_INTRODUCTION_6);
    wrefresh(introductionwin);

    dibuja_teclado(9998,1);
    wrefresh(keyboardwin);

    dibuja_manos();

    muestra_pie(opciones_pie);
    ch=getch();
    switch(ch){
        case 27:
            //ESC exit program
            flag_dentro_menus=false;
            flag_dentro_menu_lecciones=false;
            return;
            break;
    }
    wclear(introductionwin);
    wclear(handswin);
    refresh();
}

void dibuja_manos(){    
    int i,j;
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado
    handswin=subwin(mainwin,11, 80,y_keyboardwin+4, x_keyboardwin);

    int keyb_schetch[11][66]={
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,1,1,0,11,11,0,0,0,0,0,0,0,0,0,0,0,0,0,12,12,0,1,1,0,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,8,8,0,1,1,0,1,1,0,11,11,0,0,0,0,0,0,0,0,0,0,0,0,0,12,12,0,1,1,0,1,1,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,8,8,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,0,13,13,0,0,0,0,0,0,0,13,13,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,13,13,0,0,0,0,0,0,0,13,13,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    init_pair(KEYB_WHITE,COLOR_WHITE,COLOR_WHITE);
    init_pair(KEYB_BLACK,COLOR_BLACK,COLOR_BLACK);
    init_pair(KEYB_FINGER1,COLOR_WHITE,COLOR_YELLOW);
    init_pair(KEYB_FINGER2,COLOR_WHITE,COLOR_GREEN);
    init_pair(KEYB_FINGER3,COLOR_WHITE,COLOR_BLUE);
    init_pair(KEYB_FINGER4,COLOR_WHITE,COLOR_RED);
    init_pair(KEYB_FINGER5,COLOR_WHITE,COLOR_MAGENTA);    
    init_pair(KEYB_FINGER6,COLOR_WHITE,COLOR_CYAN);  
    
    for(i=0;i<11;i++){
        for(j=0;j<66;j++){
            switch(keyb_schetch[i][j]){
                case 1:
                    wattron(handswin,COLOR_PAIR(KEYB_WHITE));
                    mvwprintw(handswin,i,j,"%c",35);
                    break;
                case 8:
                    wattron(handswin,COLOR_PAIR(KEYB_FINGER1));
                    mvwprintw(handswin,i,j,"%c",32);
                    break;
                case 9:
                    wattron(handswin,COLOR_PAIR(KEYB_FINGER2));
                    mvwprintw(handswin,i,j,"%c",32);
                    break;
                case 10:
                    wattron(handswin,COLOR_PAIR(KEYB_FINGER3));
                    mvwprintw(handswin,i,j,"%c",32);
                    break;
                case 11:
                    wattron(handswin,COLOR_PAIR(KEYB_FINGER4));
                    mvwprintw(handswin,i,j,"%c",32);
                    break;
                case 12:
                    wattron(handswin,COLOR_PAIR(KEYB_FINGER5));
                    mvwprintw(handswin,i,j,"%c",32);
                    break;
                case 13:
                    wattron(handswin,COLOR_PAIR(KEYB_FINGER6));
                    mvwprintw(handswin,i,j,"%c",32);
                    break;
            }    
            
        }
    }
    wrefresh(handswin);
}

void dibuja_presentacion(){
    int i,j;
    prwin=subwin(mainwin,MAX_Y_INTRO+1, MAX_X_INTRO+1,y_prwin,x_prwin);
        
    int pr_schetch[MAX_Y_INTRO][MAX_X_INTRO]={
//                          10                  20                  30                  40                  50                  60
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0},
        {1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0,1},
        {1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,1,0,0,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0}
     };
    init_pair(KEYB_WHITE,COLOR_WHITE,COLOR_WHITE);
    init_pair(KEYB_BLACK,COLOR_BLACK,COLOR_BLACK);
    init_pair(KEYB_FINGER1,COLOR_WHITE,COLOR_YELLOW);
    init_pair(KEYB_FINGER2,COLOR_WHITE,COLOR_GREEN);
    init_pair(KEYB_FINGER3,COLOR_WHITE,COLOR_BLUE);
    init_pair(KEYB_FINGER4,COLOR_WHITE,COLOR_RED);
    init_pair(KEYB_FINGER5,COLOR_WHITE,COLOR_MAGENTA);    
    init_pair(KEYB_FINGER6,COLOR_WHITE,COLOR_CYAN);  
    
    for(i=0;i<MAX_Y_INTRO;i++){
        for(j=0;j<MAX_X_INTRO;j++){
            switch(pr_schetch[i][j]){
                case 1:
                    wattron(prwin,COLOR_PAIR(KEYB_WHITE));
                    mvwprintw(prwin,i,j,"%c",35);
                    break;
                case 8:
                    wattron(prwin,COLOR_PAIR(KEYB_FINGER1));
                    mvwprintw(prwin,i,j,"%c",32);
                    break;
                case 9:
                    wattron(prwin,COLOR_PAIR(KEYB_FINGER2));
                    mvwprintw(prwin,i,j,"%c",32);
                    break;
                case 10:
                    wattron(prwin,COLOR_PAIR(KEYB_FINGER3));
                    mvwprintw(prwin,i,j,"%c",32);
                    break;
                case 11:
                    wattron(prwin,COLOR_PAIR(KEYB_FINGER4));
                    mvwprintw(prwin,i,j,"%c",32);
                    break;
                case 12:
                    wattron(prwin,COLOR_PAIR(KEYB_FINGER5));
                    mvwprintw(prwin,i,j,"%c",32);
                    break;
                case 13:
                    wattron(prwin,COLOR_PAIR(KEYB_FINGER6));
                    mvwprintw(prwin,i,j,"%c",32);
                    break;
            }    
        }
    }
    wrefresh(prwin);    
}
