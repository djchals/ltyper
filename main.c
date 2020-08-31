#include "header.h"
#include "json_reader.h"
#include "cadenas_menus.h"
#include "menus.h"
bool flag_dentro_menus,flag_dentro_texto;
void bucle_menus();
int main(){
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
    flag_timeout=false;
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
    int i_row=0, tecla_leida, ini_w, ini_h, act_ini_w, act_ini_h;

    //creamos los pares de colores
    init_pair(C_LETRA_ERR,COLOR_WHITE,COLOR_RED);
    init_pair(C_LETRA_OK,COLOR_WHITE,COLOR_BLACK);
    init_pair(C_TIMEOUT,COLOR_RED,COLOR_BLACK);
    //

    //inicializamos las variables para la childwin y el cursor
    ancho_caja=80;
    alto_caja=10;

    ini_w=1;
    ini_h=1;
     
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
    clear();
    noecho();/*  Turn off key echoing */

    keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */  
    curs_set(0);//Ocultamos el cursor (ahora lo estoy mostrando para ver los errores)
    // 

    //iniciamos el cronometro asociando la señal SIGALRM a la función contar_segundos
	signal(SIGALRM, contar_segundos);
    childwin = subwin(mainwin, alto_caja, ancho_caja, y_child_win, x_child_win);
    box(childwin, 0, 0);

    num_cols_texto=1;
    if(long_texto>0){
        //Lo recorremos de nuevo para imprimir los caracteres dentro de la caja, ya sé que es un desastre lógico hacer el bucle dos veces pero no lo veo de otra manera, pq sino box() se lia
        for(i=0;i<long_texto;i++){
            if(todo_texto[i]==salto){
                mvwprintw(childwin,act_ini_h, act_ini_w, "%c",32);//imprimimos elcaracter
                act_ini_h++;
                act_ini_w=ini_w;
                num_cols_texto++;
            }else{
                mvwprintw(childwin,act_ini_h, act_ini_w, "%c",todo_texto[i]);//imprimimos elcaracter
                act_ini_w++;
            }
        }
        num_cols_texto++;//añadimos el salto de línea al inal para que se vea la última línea
    }
    actualiza_cursor(0,pos_h_actual,pos_w_actual,todo_texto);//mostramos el cursor en la primera letra
//     muestra_cabecera(id_texto);
    refresh();
    //iniciamos el bucle de lectura y comprobación de tecla pulsada

    muestra_cabecera(id_texto);//esta línea debe ir despues del refresh();
    int tmp_opciones[3]={1,1,1};
    muestra_pie(tmp_opciones);//esta línea debe ir despues del refresh();
    
    //esto será necesario para el tratamiento de los carácteres especiales
    unsigned char tmp_special_char[2];
    *(tmp_special_char+0)=195;
    tmp_special_char[2]=0x00;
    //   
    while(i_row<long_texto){
        ch=getch();
        if(flag_timeout){
            break;
        }
        //comprobamos las opciones del footer menu
        switch(ch){
            case 27:
                //ESC exit program
                flag_dentro_menus=false;
                salir_al_menu();
                return;
                break;
            case 0x109:/*f1 help*/break;
            case 0x10a:/*f2 cancel*/
                long_texto=i_row;
                finalizar();
                return;//lo frenamos aquí en seco, para que no se vaya al final de este while y ejecute otra vez finalizar();
                break;
            case 0x10b:/*f3 exit to menu*/
                salir_al_menu();
                return;
                break;
        }
        tecla_leida=(ch==todo_texto[i_row]);//tecla_leida=0 si es incorrecto, 1 si es correcto  

        switch(tecla_leida){
            case 0://KO tecla incorrecta
                num_errores++;
                muestra_errores();
                act_letra_err=true;

                wattron(childwin,COLOR_PAIR(C_LETRA_ERR));
                actualiza_cursor(i_row,pos_h_actual,pos_w_actual,todo_texto);

                break;
            case 1://Owrefresh(finalwin);K tecla correcta                    
                //Si la tecla pulsada no es ENTER entonces la marcamos en negrita

                if(todo_texto[i_row]!=ENTER){
                    //Marcamos el carácter en negrita
                    wattron(childwin,WA_BOLD);
                    //comprobamos si es un carácter normal o un carácter especial. Si es un carácter normal ocupa 1byte, pero si es un carácter especial ocupa 2 (á,é....)
                    if(isalnum(todo_texto[i_row]) || ispunct(todo_texto[i_row]) || isspace(todo_texto[i_row])){
                        mvwprintw(childwin,pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);
                        pos_w_actual++;//corremos una posición
                        i_row++;
                        wattron(childwin,COLOR_PAIR(C_LETRA_OK));
                        actualiza_cursor(i_row,pos_h_actual,pos_w_actual,todo_texto);
                        
                    }else{
                        //AQUÍ LOS CARACTERES ESPECIALES
                        if(todo_texto[i_row]==195){
                            *(tmp_special_char+1)=todo_texto[i_row+1];
                                mvwprintw(childwin,pos_h_actual, pos_w_actual, "%s",tmp_special_char);     
                            //si ue así es que venimos de pulsar una tecla correcta y lo anterior ue un caracter especial que ocupan el doble de bytes, por tanto sumamos i_row
                            i_row++;
                            i_row++;    
                            pos_w_actual++;//corremos una posición
                            wattron(childwin,COLOR_PAIR(C_LETRA_OK));

                            actualiza_cursor(i_row,pos_h_actual,pos_w_actual,todo_texto);
                            ch=getch();//como hemos pulsado una combinación de teclas para que salga el puto carácter leemos otra y la desecharemos        
                        }
                    }
                            actualiza_cursor(i_row,pos_h_actual,pos_w_actual,todo_texto);
                    wattroff(childwin,WA_BOLD);
                }else{
                    mvwprintw(childwin,pos_h_actual, pos_w_actual, "%c",32);//imprimimos el caracter que le queremos dar al Enter (espacio)
                    pos_w_actual=ini_w;
                    pos_h_actual++;
                    wattron(childwin,COLOR_PAIR(C_LETRA_OK));
                    i_row++;  
                    actualiza_cursor(i_row,pos_h_actual,pos_w_actual,todo_texto);
                }                            
                if(act_letra_err){
                    wattron(childwin,COLOR_PAIR(C_LETRA_OK));
                    act_letra_err=false;
                }
                break;
        }
        wrefresh(childwin);
    }
    finalizar();
}
void actualiza_cursor(int i_row, int pos_h_actual, int pos_w_actual, unsigned char todo_texto[]){
    mvprintw(20, 0, " ");//si comento esta línea da errores el iltro
    wrefresh(childwin);
    
    wattron(childwin,WA_UNDERLINE);
    wattroff(childwin,WA_BOLD);
    if(todo_texto[i_row]==ENTER){
        mvwprintw(childwin,pos_h_actual, pos_w_actual, "%c",32);        
    }else if(isalnum(todo_texto[i_row]) || ispunct(todo_texto[i_row]) || isspace(todo_texto[i_row])){
        mvwprintw(childwin,pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);
    }else{
        //AQUÍ LOS CARACTERES ESPECIALES
        if(todo_texto[i_row]==195){
            unsigned char tmp_special_char[2];
            tmp_special_char[2]=0x00;
            *(tmp_special_char+0)=195;
            *(tmp_special_char+1)=todo_texto[i_row+1];
            mvwprintw(childwin,pos_h_actual, pos_w_actual, "%s",tmp_special_char);           
        }
    }
    wattroff(childwin,WA_UNDERLINE);
}
void muestra_errores(void){
    wrefresh(errorwin);
    wattron(errorwin,COLOR_PAIR(C_LETRA_OK));
    mvwprintw(errorwin,0, 0, "Errores: %d",num_errores);
    wrefresh(errorwin);    
}
void muestra_cabecera(int id_texto){
    mvprintw(0, 0, obten_titulo(id_texto));
    
    //Creamos las ventana de tiempo y errores ya que se tendrán que ir rerescando periodicamente
    timewin = newwin(1, 15, 0, 50);
    errorwin = newwin(1, 15, 0, 30);
    
    muestra_errores();
    contar_segundos();

    mvprintw(POS_H_CABECERA, POS_W_CABECERA, "Repite el texto que ves a continuación:");
}
void muestra_pie(int opciones[3]){
    //según las opciones[] que traigamos mostraremos unas opciones del menú u otras
    
    char var_barra[max_x];

    //creamos los pares de colores
    init_pair(C_LETRA_PIE,COLOR_WHITE,COLOR_BLUE);

    footerwin=newwin(1, max_x, max_y-1,0);    

    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    
    wattron(footerwin,COLOR_PAIR(C_LETRA_PIE));
    mvwprintw(footerwin,0, 0,"%s",var_barra);
    
    char et_opcion0[]="Salir del programa";

    char et_opcion11[]="Cancelar texto";

    char et_opcion12[]="Repetir texto";

    char et_opcion2[]="Volver al menú";
    
    int pos_opc=1;//with this variable we control the options position in the menu
    if(opciones[0]){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"ESC");
        pos_opc=pos_opc+3;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0, pos_opc,et_opcion0);
        pos_opc=pos_opc+strlen(et_opcion0)+3;
    }
    if(opciones[1]==1){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"F2");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0, pos_opc,et_opcion11);
        pos_opc=pos_opc+strlen(et_opcion11)+3;
    }else if(opciones[1]==2){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"R");
        pos_opc++;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0, pos_opc,et_opcion12);
        pos_opc=pos_opc+strlen(et_opcion12)+3;        
    }
    if(opciones[2]){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0,pos_opc,"F3");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0,pos_opc,et_opcion2);
        pos_opc=pos_opc+strlen(et_opcion2)+3;
    }
    
    wattroff(footerwin,COLOR_PAIR(C_LETRA_PIE));
    wrefresh(footerwin);
}
// Esta es la función que se va a ejecutar cada vez que se reciba la señal SIGALRM
void contar_segundos(){
    //creamos los pares de colores
    // Usamos static para que se conserve el valor de "segundos" entre cada llamada a la función
	total_tiempo++;
    minutos=floor(total_tiempo/60);
    segundos=total_tiempo%60;   

    wattron(timewin,COLOR_PAIR(C_LETRA_OK));
    wattroff(timewin,WA_BLINK);
        
    mvwprintw(timewin,0, 0, "Tiempo: ");

    if((MAX_TIEMPO-total_tiempo)<30){
        wattron(timewin,COLOR_PAIR(C_TIMEOUT));
        wattron(timewin,WA_BLINK);
    }
    mvwprintw(timewin,0, 8, "%02d:%02d",minutos,segundos);
    wrefresh(timewin);

    if(total_tiempo<MAX_TIEMPO){
        alarm(1);
    }else{
        flag_timeout=true;//se acabó el tiempo! 
        //
        
        char tmp_cadena[]="SE TE HA ACABADO EL TIEMPO!!";
        int tmp_borde=floor((ancho_caja-strlen(tmp_cadena))/2);
        wattron(childwin,WA_BLINK);
        wattron(childwin,WA_BOLD);
        mvwprintw(childwin,4,tmp_borde,tmp_cadena);
        wattroff(childwin,WA_BLINK);
        wattroff(childwin,WA_BOLD);
        
        mvprintw(13, 0, "Presiona cualquier tecla para continuar...");
        wrefresh(childwin);
        refresh();
    
    }
}
void finalizar(){
    //PARAMOS EL CRONOMETRO
    alarm(0);
    flag_salir=true;
    ungetch(ch);
    bool flag_opcion_valida=false;
    float minutos_reales=(float) total_tiempo/60;
    float num_ppm=(long_texto+num_errores)/minutos_reales;

    clear();

    finalwin = subwin(mainwin,7, 80, y_child_win, x_child_win);
    box(finalwin, 0, 0);   

    mvprintw(2, 0, "Has obtenido la siguiente puntuación:");
    mvwprintw(finalwin,2, 1, "Pulsaciones/min");
    mvwprintw(finalwin,2, 20, "%d",(int)num_ppm);
    mvwprintw(finalwin,3, 1, "Errores");
    mvwprintw(finalwin,3, 20, "%d",num_errores);
    mvwprintw(finalwin,4, 1, "Tiempo");
    mvwprintw(finalwin,4, 20, "%02d:%02d",minutos,segundos);

    int tmp_opciones[3]={1,2,1};
    wrefresh(finalwin);
    refresh();
    muestra_pie(tmp_opciones);    

   do{
        ch=getch();
        //comprobamos las opciones del footer menu
        switch(ch){
            case 27:
                //ESC exit program
                flag_opcion_valida=true;
                flag_dentro_menus=false;
                salir_al_menu();
                return;
                break;
            case 'r':
            case 'R':
                //R repeat the text
                flag_opcion_valida=true;
                //do need anymore, whe are in a loop 
                break;
            case 0x10b:/*f3 exit to menu*/
                flag_opcion_valida=true;
                salir_al_menu();
                return;
                break;
        }
   }while(!flag_opcion_valida);
    
}
void salir_al_menu(){
    alarm(0);
    flag_dentro_texto=false;
    delwin(childwin);    
    endwin();    
}

void pitar(void){
    //Más adelante configuraremos si queremos que pite o no
    //beep();
}
