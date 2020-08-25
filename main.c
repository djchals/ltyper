#include "header.h"
#include "json_reader.h"
#include "cadenas_menus.h"
#include "menus.h"

// void borrar_caja(int y, int x,int alto_caja);
bool flag_dentro_menus,flag_dentro_texto;
void bucle_menus();
int main(){
    
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
    
    _init_ncurses();//necesario para mostrar los menus y las cajas, etc...
    _init_json();//necesario para leer el archivo db_texts.json
    bucle_menus();
    return 0;
}
void bucle_menus(){
    flag_dentro_menus=true;

    int opcion_selec,opcion_sub_sel;

    while(flag_dentro_menus){
        opcion_selec=0;
        opcion_sub_sel=0;
        
        opcion_selec=muestra_menu(0);
        switch(opcion_selec){
            case 0:/*muestra_lecciones*/
                clear();
                opcion_sub_sel=muestra_menu(1);
                if(opcion_sub_sel!=5 && opcion_sub_sel!=6){
                    flag_dentro_texto=true;//con este flag controlamos si estamos escribiendo un texto y lo estamos repitiendo
                    while(flag_dentro_texto){
                        clear();
                        muestra_texto(opcion_sub_sel);
                    }
                } 
                break;
            case 1:/*texto personalizado*/break;
            case 2:/*configurar*/break;
            case 3:/*Salir*/
                flag_dentro_menus=false;
                break;   
        }
    }
}

void muestra_texto(int act_id_texto){
    //inicializamos de nuevo las variables globales:
    total_tiempo=-1;
    pos_w_actual=0;
    pos_h_actual=0;
    act_letra_err=false;//con esta controlaremos si nos hemos equivocado en la letra actual
    minutos=0;
    segundos=0;
    num_errores=0;
    long_texto=0;
    id_texto=act_id_texto;//guardamos el valor de la id del texto que vamos a leer en la variable global id_texto
    //
    
    //mostramos el texto que hay que repetir
    char *array_puntero_texto=obten_texto(act_id_texto);//id_texto es una variable global
    //     return 0;

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
    ancho_caja=70;
    alto_caja=7;
    
    //Creamos la ventana
    childwin = subwin(mainwin, alto_caja, ancho_caja, y_child_win, x_child_win);
    box(childwin, 0, 0);   
    //iniciamos el cronometro asociando la señal SIGALRM a la función contar_segundos
	signal(SIGALRM, contar_segundos);
    muestra_cabecera(id_texto);

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
        tecla_leida=leer_tecla(todo_texto[i_row]);
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
int leer_tecla(int comprueba_letra) {
    int ch=getch();
    return (ch==comprueba_letra);
}
void muestra_errores(void){
    attron(COLOR_PAIR(C_LETRA_OK));
    mvprintw(0, 30, "Errores: %d",num_errores);
}
void muestra_cabecera(int id_texto){
    mvprintw(0, 0, obten_titulo(id_texto));
    muestra_errores();
    contar_segundos();
    mvprintw(POS_H_CABECERA, POS_W_CABECERA, "Repite el texto que ves a continuación:");
}
// Esta es la función que se va a ejecutar cada vez que se reciba la señal SIGALRM
void contar_segundos(){
	// Usamos static para que se conserve el valor de "segundos" entre cada llamada a la función
	total_tiempo++;
    
    minutos=floor(total_tiempo/60);
    segundos=total_tiempo%60;    
    
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
    //PARAMOS EL CRONOMETRO
    alarm(0);

    int ch;
    float minutos_reales=(float) total_tiempo/60;
    float num_ppm=(long_texto+num_errores)/minutos_reales;
//     borrar_caja(y_child_win,x_child_win,alto_caja);
clear();
    childwin = subwin(mainwin, 7, 70, y_child_win, x_child_win);
    box(childwin, 0, 0);   
    
    mvprintw(2, 0, "Has obtenido la siguiente puntuación:");
    mvprintw(5, 1, "Pulsaciones/min");
    mvprintw(5, 20, "%d",(int)num_ppm);
    mvprintw(6, 1, "Errores");
    mvprintw(6, 20, "%d",num_errores);
    mvprintw(7, 1, "Tiempo");
    mvprintw(7, 20, "%02d:%02d",minutos,segundos);
    mvprintw(10, 0, "Desea repetir el texto actual? (S/n): ");

    
    refresh();
    while(ch!='n' && ch!='N' && ch!='s' && ch!='S'){ 
        ch=getch();
    }
    switch(ch){
        case 's':
        case 'S':
            //para repetirlo no hay que hacer nada, ya solos iremos a repetirlo debido a flag_dentro_texto
            break;

        case 'n':
        case 'N':
            flag_dentro_texto=false;
            delwin(mainwin);    
            endwin();
            break;
    }
}
void pitar(void){
    //Más adelante configuraremos si queremos que pite o no
    //beep();
}
