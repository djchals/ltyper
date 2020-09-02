#include "header.h"
#include "json_reader.h"
#include "menus.h"

int main(){
    bucle_menus();
    return 0;
}
void bucle_menus(){
    flag_dentro_menus=true;
    int opcion_selec,lecccion_sel;
    _init_cursos();//necesario para leer el archivo json
    while(flag_dentro_menus){
        flag_dentro_menu_lecciones=true;
        opcion_selec=0;
        lecccion_sel=0;
        opcion_selec=muestra_menu(0);
        while(flag_dentro_menu_lecciones && opcion_selec!=9999){
            lecccion_sel=muestra_menu(array_cursos[opcion_selec]);//aquí ya deiniremos id_course
            flag_dentro_texto=true;//con este flag controlamos si estamos escribiendo un texto y lo estamos repitiendo
            clear();
            while(flag_dentro_texto && lecccion_sel!=9999){
                muestra_texto(lecccion_sel,array_cursos[opcion_selec]);
            }
        }
    }
    refresh();
    endwin();//si llegamos aquí es que hemos pulsado la tecla ESC en los menús
}

void muestra_texto(int act_id_texto, int id_course){
    
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
    char *array_puntero_texto=obten_texto(act_id_texto,id_course);//id_texto es una variable global
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

    muestra_cabecera(id_texto,id_course);//esta línea debe ir despues del refresh();
    int tmp_opciones[4]={1,1,1,1};
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
                seleccionar_otra_leccion();
                return;
                break;
            case 0x109:/*f1 help*/break;
            case 0x10a:/*f2 cancel*/
                long_texto=i_row;
                finalizar(id_course);
                return;//lo frenamos aquí en seco, para que no se vaya al final de este while y ejecute otra vez finalizar();
                break;
            case 0x10b:/*f3 change lesson*/
                seleccionar_otra_leccion();
                return;
                break;
            case 0x10c:/*f4 change course*/
                flag_dentro_menu_lecciones=false;
                seleccionar_otra_leccion();
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
    finalizar(id_course);
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
    mvwprintw(errorwin,0, 0, ET_ERRORS": %d",num_errores);
    wrefresh(errorwin);    
}
void muestra_cabecera(int id_texto, int id_course){
    //Creamos las ventana de tiempo y errores ya que se tendrán que ir rerescando periodicamente
    timewin = newwin(1, 15, 1, 50);
    errorwin = newwin(1, 15, 1, 30);
    muestra_errores();
    contar_segundos();
    muestra_titulo_curso(id_course);

    
    mvprintw(1, 0, obten_titulo(id_texto,id_course));
    mvprintw(POS_H_CABECERA, POS_W_CABECERA, ET_REPEAT_THE_TEXT);
}

void muestra_titulo_curso(int id_course){
    char var_barra[max_x];
    init_pair(C_TITLE,COLOR_WHITE,COLOR_MAGENTA);
    
    attron(A_BOLD);
    attron(COLOR_PAIR(C_TITLE));
    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    mvprintw(0, 0,"%s",var_barra);
    int tmp_borde=floor((ancho_caja-strlen(array_et_course_title[id_course]))/2);
    mvprintw(0, tmp_borde, "%s",array_et_course_title[id_course]);
    attroff(COLOR_PAIR(C_TITLE));
    attroff(A_BOLD);

    refresh();  
}
void muestra_pie(int opciones[4]){    
    //según las opciones[] que traigamos mostraremos unas opciones del menú u otras
    char var_barra[max_x];

    //creamos los pares de colores
    init_pair(C_LETRA_PIE,COLOR_WHITE,COLOR_BLUE);
    footerwin=newwin(1, max_x, max_y-1,0);    
    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    
    wattron(footerwin,COLOR_PAIR(C_LETRA_PIE));
    mvwprintw(footerwin,0, 0,"%s",var_barra);
    
    char et_opcion0[]=ET_OPTION0;
    char et_opcion11[]=ET_OPTION11;
    char et_opcion12[]=ET_OPTION12;
    char et_opcion2[]=ET_OPTION2;
    char et_opcion3[]=ET_OPTION3;   
    
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
    if(opciones[3]){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0,pos_opc,"F4");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0,pos_opc,et_opcion3);
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
        
    mvwprintw(timewin,0, 0, ET_TIME": ");

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
        char tmp_cadena[]=ET_TIME_OVER;
        int tmp_borde=floor((ancho_caja-strlen(tmp_cadena))/2);
        wattron(childwin,WA_BLINK);
        wattron(childwin,WA_BOLD);
        mvwprintw(childwin,5,tmp_borde,tmp_cadena);
        wattroff(childwin,WA_BLINK);
        wattroff(childwin,WA_BOLD);
        
        mvprintw(14, 0, ET_PRESS_KEY_CONTINUE);
        wrefresh(childwin);
        refresh();
    }
}
void finalizar(int id_course){
    //PARAMOS EL CRONOMETRO
    alarm(0);

    clear();
    flag_salir=true;
    ungetch(ch);
    bool flag_opcion_valida=false;
    float minutos_reales=(float) total_tiempo/60;
    float num_ppm=(long_texto+num_errores)/minutos_reales;

    finalwin = subwin(mainwin,8, 80, y_child_win, x_child_win);
    box(finalwin, 0, 0);   

    mvprintw(3, 0, ET_YOUR_SCORE);
    mvwprintw(finalwin,2, 20, "%s",obten_titulo(id_texto,id_course));
    mvwprintw(finalwin,3, 1, ET_PPM );
    mvwprintw(finalwin,3, 20, "%d",(int)num_ppm);
    mvwprintw(finalwin,4, 1, ET_ERRORS);
    mvwprintw(finalwin,4, 20, "%d",num_errores);
    mvwprintw(finalwin,5, 1, ET_TIME);
    mvwprintw(finalwin,5, 20, "%02d:%02d",minutos,segundos);

    int tmp_opciones[4]={1,2,1,1};
    wrefresh(finalwin);

    muestra_titulo_curso(id_course);
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
                flag_dentro_menu_lecciones=false;
                seleccionar_otra_leccion();
                return;
                break;
            case 'r':
            case 'R':
                //R repeat the text
                flag_opcion_valida=true;
                //do need anymore, whe are in a loop 
                break;
            case 0x10b:/*f3 change lesson*/
                flag_opcion_valida=true;
                seleccionar_otra_leccion();
                return;
                break;
            case 0x10c:/*f4 change course*/
                flag_opcion_valida=true;
                flag_dentro_menu_lecciones=false;
                seleccionar_otra_leccion();
                return;
                break;
            
        }
   }while(!flag_opcion_valida);    
}
void seleccionar_otra_leccion(){
    alarm(0);
    flag_dentro_texto=false;
    delwin(childwin);    
    endwin();    
}

void pitar(void){
    //Más adelante configuraremos si queremos que pite o no
    //beep();
}
