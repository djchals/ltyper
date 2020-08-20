#include "header.h"
#include "lectorxml.h"

int main(){    
    //mostramos el texto que hay que repetir
    char *array_puntero_texto;
    int long_texto;
    array_puntero_texto=obten_texto(0);
    long_texto=strlen(array_puntero_texto);
    
    int salto = 0x0a;//código de la tecla enter
    int num_cols_texto=0,i=0;

    char todo_texto[long_texto];
    memcpy(todo_texto,array_puntero_texto,long_texto);

    int i_row;
    int tecla_leida;
    int ini_w,ini_h,act_ini_w,act_ini_h;
// 
    ini_w=1;
    ini_h=4;
//     
    act_ini_w=ini_w;
    act_ini_h=ini_h;    

    pos_w_actual=act_ini_w;
    pos_h_actual=act_ini_h;    

    int width=70, height=7;//dimensiones de la ventana

    /*  Initialize ncurses  */
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initializing ncurses.\n");
        exit(EXIT_FAILURE);
    }    
    
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(EXIT_FAILURE);
    }
    if(start_color()!=OK) {
        endwin();
        printf("Your terminal cannot start colors\n");
        exit(EXIT_FAILURE);
    }

    //creamos los pares de colores
    init_pair(C_LETRA_ERR,COLOR_WHITE,COLOR_RED);
    init_pair(C_LETRA_OK,COLOR_WHITE,COLOR_BLACK);
    init_pair(C_TIMEOUT,COLOR_RED,COLOR_BLACK);
    //
    noecho();                  /*  Turn off key echoing                 */
    keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */  
    //Ocultamos el cursor
    curs_set(1);
// 
    //Creamos la ventana
    childwin = subwin(mainwin, height, width, y_child_win, x_child_win);
    box(childwin, 0, 0);   
    //iniciamos el cronometro asociando la señal SIGALRM a la función contar_segundos
	signal(SIGALRM, contar_segundos);
    muestra_cabecera();

    if(long_texto>0){
        //Hay algo de texto
        num_cols_texto=1;
        for(i=0;i<long_texto;i++){
            if(todo_texto[i]==salto){
                num_cols_texto++;
                act_ini_h++;
                act_ini_w=ini_w;
            }else{
                mvprintw(act_ini_h, act_ini_w, "%c",todo_texto[i]);//imprimimos la línea
                act_ini_w++;
            }
            
        }
    }
    move(pos_h_actual,pos_w_actual);
    redrawwin(childwin);
// 
//  //iniciamos el bucle de lectura y comprobación de tecla pulsada
    i_row=0;//,i_col=0;
    while(i_row<long_texto){
        tecla_leida=leer_tecla(todo_texto[i_row],i_row);
        switch(tecla_leida){
            case 0://KO tecla incorrecta
                curs_set(0);//desactivamos el cursor pq no queremos que se vea como se mueve durante este proceso
                num_errores++;
                muestra_errores();
                pitar();
                act_letra_err=true;
                move(pos_h_actual,pos_w_actual);// colocamos el cursor en la posición que le toca
                attron(COLOR_PAIR(C_LETRA_ERR));
                if(todo_texto[i_row]!=ENTER){
                    mvprintw(pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);//Repetimos el carácter correcto
                }else{
                    mvprintw(pos_h_actual, pos_w_actual, " ");//Escribimos un espacio 
                }
                curs_set(1);//volvemos a activar el cursor y que siga el juego
                break;
            case 1://OK tecla correcta                    
                //Si la tecla pulsada no es ENTER entonces la marcamos en negrita
                if(todo_texto[i_row]!=ENTER){
                    //Marcamos el carácter en negrita
                    attron(A_BOLD);
                    mvprintw(pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);
                    attroff(A_BOLD);
                    pos_w_actual++;//corremos una posición
                }else{
                        pos_w_actual=ini_w;
                        pos_h_actual++;
                }
                i_row++;
                if(act_letra_err){
                    attron(COLOR_PAIR(C_LETRA_OK));
                    act_letra_err=false;
                }
                move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición
                break;
        }
    }
    finalizar();
}
int leer_tecla(int comprueba_letra, int pos_w_actual) {
    int ch=getch();
    return (ch==comprueba_letra);
}
void muestra_errores(void){
    attron(COLOR_PAIR(C_LETRA_OK));
    mvprintw(0, 30, "Errores: %d",num_errores);
}
void muestra_cabecera(void){
    mvprintw(0, 0, "F1=Salir");
    muestra_errores();
    contar_segundos();
    mvprintw(2, 0, "Repite el texto que ves a continuación:");
}
// Esta es la función que se va a ejecutar cada vez que se reciba la señal SIGALRM
void contar_segundos(){
	// Usamos static para que se conserve el valor de "segundos" entre cada llamada a la función
	total_tiempo++;
    
    int minutos=floor(total_tiempo/60);
    int segundos=total_tiempo%60;    
    
    attron(COLOR_PAIR(C_LETRA_OK));
    mvprintw(0, 50, "Tiempo: ");

    if((MAX_TIEMPO-total_tiempo)<30){
        attron(COLOR_PAIR(C_TIMEOUT));
        attron(A_BLINK);
    }
    mvprintw(0, 58, "%02d:%02d",minutos,segundos);
    if((MAX_TIEMPO-total_tiempo)<30){
        attron(COLOR_PAIR(C_LETRA_OK));
        attroff(A_BLINK);
    }
    
    move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición

    //SI se ha cometido un error dejamos el color tal y como estaba
    if(act_letra_err){
        attron(COLOR_PAIR(C_LETRA_ERR));
    }
    
    refresh();
    if(total_tiempo<MAX_TIEMPO){
        alarm(1);
    }else{
        finalizar();//se acabó el tiempo!   
    }
}
void finalizar(){
    /*  Clean up after ourselves  */
//     int ch;
//     delwin(childwin);
//     refresh();
//     
//     int width=60, height=4;//dimensiones de la ventana
//     //Creamos la ventana de nuevo
//     childwin = subwin(mainwin, height, width, y_child_win, x_child_win);
//     box(childwin, 0, 0);
// 
//     mvaddstr(5, 10, "Has conseguido la siguiente puntuación:");
//     refresh();
//     
//     while ( (ch = getch()) != 'q' ) {
//     }
//     delwin(mainwin);    
//     
//     endwin();
//     
//     refresh();
//     printf("Adiosss");
//     exit(EXIT_SUCCESS);
}
void pitar(void){
    //Más adelante configuraremos si queremos que pite o no
//     beep();
}
