#include "header.h"
#include "json_reader.h"
#include "cadenas_menus.h"
#include "menus.h"

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
    //inicializamos de nuevo las variables globales
    total_tiempo=-1;
    pos_w_actual=0;
    pos_h_actual=0;
    act_letra_err=false;//con esta controlaremos si nos hemos equivocado en la letra actual
    minutos=0;
    segundos=0;
    num_errores=0;
    long_texto=0;
    id_texto=act_id_texto;//guardamos el valor de la id del texto que vamos a leer en la variable global id_texto
    int salto=0x0a;//código de la tecla enter
    int num_cols_texto=0,i=0;
    int i_row, tecla_leida, ini_w, ini_h, act_ini_w, act_ini_h;
    //

    //creamos los pares de colores
    init_pair(C_LETRA_ERR,COLOR_WHITE,COLOR_RED);
    init_pair(C_LETRA_OK,COLOR_WHITE,COLOR_BLACK);
    init_pair(C_TIMEOUT,COLOR_RED,COLOR_BLACK);
    //

    //inicializamos las variables para la childwin y el cursor
    ancho_caja=70;
    alto_caja=1;

    ini_w=1;
    ini_h=4;
     
    act_ini_w=ini_w;
    act_ini_h=ini_h;    

    pos_w_actual=act_ini_w;
    pos_h_actual=act_ini_h;   
    //

    //obtenemos el texto que hay que repetir
    char *array_puntero_texto=obten_texto(act_id_texto);//id_texto es una variable global
    long_texto=strlen(array_puntero_texto);
    unsigned char todo_texto[long_texto];
    memcpy(todo_texto,array_puntero_texto,long_texto);
    //

    //preparamos el terminal para el modo menú/lectura de tecla
    noecho();                  /*  Turn off key echoing                 */
    keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */  
    curs_set(1);//Ocultamos el cursor
    // 

    //iniciamos el cronometro asociando la señal SIGALRM a la función contar_segundos
	signal(SIGALRM, contar_segundos);
    childwin = subwin(mainwin, alto_caja, ancho_caja, y_child_win, x_child_win);

    num_cols_texto=1;
    if(long_texto>0){
        //Lo recorremos de nuevo para imprimir los caracteres dentro de la caja, ya sé que es un desastre lógico hacer el bucle dos veces pero no lo veo de otra manera, pq sino box() se lia
        for(i=0;i<long_texto;i++){
            if(todo_texto[i]==salto){
                mvprintw(act_ini_h, act_ini_w, "%c",todo_texto[i]);//imprimimos elcaracter

                act_ini_h++;
                act_ini_w=ini_w;
                num_cols_texto++;
            }else{                
                mvprintw(act_ini_h, act_ini_w, "%c",todo_texto[i]);//imprimimos elcaracter
                act_ini_w++;
            }
        }
        num_cols_texto++;//añadimos el salto de línea al inal para que se vea la última línea
    }
    wresize(childwin,alto_caja+num_cols_texto,ancho_caja);
    box(childwin, 0, 0);
    muestra_cabecera(id_texto);
    move(pos_h_actual,pos_w_actual);
 
    //iniciamos el bucle de lectura y comprobación de tecla pulsada
    i_row=0;int ch;unsigned char tmp_special_char[2];
    
    //esto será necesario para el tratamiento de los carácteres especiales
    *(tmp_special_char+0)=195;
    tmp_special_char[2]=0x00;
    //
    
    while(i_row<long_texto){
        ch=getch();
        move(pos_h_actual,pos_w_actual);// colocamos el cursor en la posición que le toca

        
        tecla_leida=(ch==todo_texto[i_row]);//tecla_leida=0 si es incorrecto, 1 si es correcto

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
                    //AQUÍ LOS CARACTERES ESPECIALES
                    if(todo_texto[i_row]==195){
                        *(tmp_special_char+1)=todo_texto[i_row+1];
                        mvprintw(pos_h_actual, pos_w_actual, "%s",tmp_special_char);         
                    }else{
                        mvprintw(pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);//Repetimos el carácter correcto
                    }
                }else{
                    mvprintw(pos_h_actual, pos_w_actual, "%c"," ");//Escribimos un espacio 
                }
                curs_set(1);//volvemos a activar el cursor y que siga el juego
                break;
            case 1://OK tecla correcta                    
                //Si la tecla pulsada no es ENTER entonces la marcamos en negrita
                if(todo_texto[i_row]!=ENTER){
                    //Marcamos el carácter en negrita
                    attron(A_BOLD);
                    //comprobamos si es un carácter normal o un carácter especial. Si es un carácter normal ocupa 1byte, pero si es un carácter especial ocupa 2 (á,é....)
                    if(isalnum(todo_texto[i_row]) || ispunct(todo_texto[i_row]) || isspace(todo_texto[i_row])){
                        mvprintw(pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);
                    }else{
                        //AQUÍ LOS CARACTERES ESPECIALES
                        if(todo_texto[i_row]==195){
                            *(tmp_special_char+1)=todo_texto[i_row+1];
                            mvprintw(pos_h_actual, pos_w_actual, "%s",tmp_special_char);     
                                        //si ue así es que venimos de pulsar una tecla correcta y lo anterior ue un caracter especial que ocupan el doble de bytes, por tanto sumamos i_row
                            i_row++;
                            ch=getch();//como hemos pulsado una combinación de teclas para que salga el puto carácter leemos otra y la desecharemos        
                        }
                    }
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
                mvprintw(27, 0, "uego al 21");//imprimimos elcaracte si comento esta linea sale el error si me equivoco de caracter

    }

    finalizar();
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
