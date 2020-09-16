#include "header.h"
#include "json_reader.h"
#include "menus.h"
#include "draw_keys.h"
#include "coord_wins.h"

int main(){
    setlocale(LC_ALL,"es_ES");
    _init_cursos();//necesario para leer el archivo json en json_reader.h
    _init_ncurses();//it s not the same
    obten_coord_wins();
    bucle_menus();
    return 0;
}
void bucle_menus(){
    flag_dentro_menus=true;
    int opcion_selec,lecccion_sel;
    
    titlewin=newwin(1, max_x, y_titlewin,y_titlewin);  
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
//  init_pair(C_LETRA_PRUEBA,COLOR_WHITE,COLOR_RED);
    init_pair(C_LETRA_PRUEBA,COLOR_WHITE,COLOR_GREEN);
    
    //
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
    childwin = subwin(mainwin, alto_caja, ancho_caja, y_childwin, x_childwin);
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
//     actualiza_cursor(0,pos_h_actual,pos_w_actual,3,todo_texto);//mostramos el cursor en la primera letra
//     dibujamos el teclado
    dibuja_teclado(id_texto,id_course);   
    //
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
    int conta=0;
//     int act_pos=0;//con esta controlamos la posición en el string, los caracteres especiales aquí serán solo 1
    //en i_row son dobles!
    int act_attrs;
    act_letra_err=false;
    while(i_row<long_texto){ 
        if(todo_texto[i_row]==195){
            i_row++;
            continue;
        }  
        actualiza_cursor(i_row,pos_h_actual,pos_w_actual,3,todo_texto);
   
        conta++;
        //marcamos la tecla que debemos pulsar
        if(todo_texto[i_row]==ENTER){
            marca_blink_letra(64,true); 
        }else if(isalnum(todo_texto[i_row]) || ispunct(todo_texto[i_row]) || isspace(todo_texto[i_row])){
            marca_blink_letra(todo_texto[i_row],true); 
        }
        ch=getch();
        wrefresh(childwin);
        if(flag_timeout){
            break;
        }
        //comprobamos las opciones del footer menu
        switch(ch){
            case 27:
                //ESC exit program
                flag_dentro_menus=false;
                flag_dentro_menu_lecciones=false;
                seleccionar_menu();
                return;
                break;
            case 0x109:/*f1 help*/break;
            case 0x10a:/*f2 cancel*/
                long_texto=i_row;
                finalizar(id_course);
                return;//lo frenamos aquí en seco, para que no se vaya al final de este while y ejecute otra vez finalizar();
                break;
            case 0x10b:/*f3 change lesson*/
                seleccionar_menu();
                return;
                break;
            case 0x10c:/*f4 change course*/
                flag_dentro_menu_lecciones=false;
                seleccionar_menu();
                return;
        }
        tecla_leida=comprueba_tecla(ch,i_row,todo_texto);//tecla_leida=0 si es incorrecto, 1 si es correcto  
        switch(tecla_leida){
            case 0://KO tecla incorrecta
                    if(ch!=195){
                        num_errores++;
                        muestra_errores();
                        act_letra_err=true;
                        actualiza_cursor(i_row,pos_h_actual,pos_w_actual,2,todo_texto);
                    }
                break;
            case 1://OK tecla correcta  
                if(act_letra_err){
                    act_attrs=0;
                    act_letra_err=false;
                }else{
                    act_attrs=1;
                }
                //Si la tecla pulsada no es ENTER entonces la marcamos en negrita
                if(todo_texto[i_row]!=ENTER){
                    marca_blink_letra(todo_texto[i_row],false); //desmarcamos la última tecla pulsada
                    actualiza_cursor(i_row,pos_h_actual,pos_w_actual,act_attrs,todo_texto);
                    pos_w_actual++;//corremos una posición
                    
                }else{
                    //desmarcamos la última tecla pulsada
                    marca_blink_letra(64,false);
                    //
                    actualiza_cursor(i_row,pos_h_actual,pos_w_actual,act_attrs,todo_texto);
                    pos_w_actual=ini_w;
                    pos_h_actual++;
                }
//                 act_pos++;//sumamos una posición 
                i_row++;
//                 i_row=act_pos+contar_195(i_row,todo_texto);
                break;
        }
//         wmove(childwin,pos_h_actual,pos_w_actual);
        wrefresh(childwin);
    }
    finalizar(id_course);
}
bool is_special(int tmp_caracter){
    switch(tmp_caracter){
        case 161://á
        case 169://é
        case 173://í
        case 179://ó
        case 186://ú
        case 129://Á
        case 137://É
        case 141://Í
        case 147://Ó
        case 154://Ú
        case 177://ñ
        case 145://Ñ
            return true;
        break;
        default:
            return false;
        break;
    }
}
int comprueba_tecla(int ch, int i_row, unsigned char todo_texto[]){
     if(ch<=128){
         return (int)(ch==todo_texto[i_row]);
    }
    switch(ch){
        case 161://á
        case 169://é
        case 173://í
        case 179://ó
        case 186://ú
        case 129://Á
        case 137://É
        case 141://Í
        case 147://Ó
        case 154://Ú
        case 177://ñ
        case 145://Ñ
            return (int)(ch==todo_texto[i_row]);
            break;
        default:
            return 0;
            break;
    }
}

void actualiza_cursor(int i_row, int pos_h_actual, int pos_w_actual, int flag_attrs,unsigned char todo_texto[]){
    wattrset(childwin,0);
    switch(flag_attrs){
        case 0:wattron(childwin,WA_BOLD | COLOR_PAIR(C_LETRA_ERR));break;
        case 1:wattron(childwin,WA_BOLD | COLOR_PAIR(C_LETRA_OK));break;
        case 2:wattron(childwin,WA_BLINK | WA_REVERSE | COLOR_PAIR(C_LETRA_ERR));break;
        case 3:wattron(childwin,WA_BLINK | WA_REVERSE | COLOR_PAIR(C_LETRA_OK));break;
    }
    if(todo_texto[i_row]==ENTER){
        mvwprintw(childwin,pos_h_actual, pos_w_actual, "%c",32);   
        marca_blink_letra(64,true);
    }else if(isprint(todo_texto[i_row])){
        mvwprintw(childwin,pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);
    }else{
        //AQUÍ LOS CARACTERES ESPECIALES
        if(is_special(todo_texto[i_row])){
            unsigned char tmp_special_char[2];
            *(tmp_special_char+0)=195;
            *(tmp_special_char+1)=todo_texto[i_row];
            mvwprintw(childwin,pos_h_actual, pos_w_actual, "%s",tmp_special_char);
        }
    }
    wrefresh(childwin);
}
void muestra_errores(void){
//  wrefresh(errorwin);
    wattron(errorwin,COLOR_PAIR(C_LETRA_OK));
    mvwprintw(errorwin,0, 0, ET_ERRORS": %d",num_errores);
    wrefresh(errorwin);    
}
void muestra_cabecera(int id_texto, int id_course){
    //Creamos las ventana de tiempo y errores ya que se tendrán que ir rerescando periodicamente
    timewin = newwin(1, 15, y_timewin, x_timewin);
    errorwin = newwin(1, 15, y_errorwin, x_errorwin);
    lessonwin=newwin(1, max_x, y_lessonwin,x_lessonwin);  
    WINDOW *descwin;
    descwin=newwin(1, max_x, y_descwin,x_descwin);  
    //
    
    muestra_titulo_curso(id_course);
    mvwprintw(lessonwin,0, 0,"%s %s",ET_LESSON, obten_titulo(id_texto,id_course));
    wrefresh(lessonwin);

    mvwprintw(descwin,0,0, ET_DESC);
    wrefresh(descwin);

    
    muestra_errores();

    contar_segundos();
    wrefresh(timewin);

    
//     wrefresh(footerrwin);

    
}
void muestra_titulo_curso(int id_course){
    wrefresh(titlewin);
    char var_barra[max_x];
    
    char act_titulo_curso[255];
    strcpy(act_titulo_curso,array_et_course_title[id_course]);
    strcat(act_titulo_curso," - ");
    strcat(act_titulo_curso,ET_PROGRAMA);
    
    init_pair(C_TITLE,COLOR_WHITE,COLOR_MAGENTA);
    int tmp_borde=floor((max_x-strlen(act_titulo_curso))/2);

    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    wattron(titlewin, WA_BOLD | COLOR_PAIR(C_TITLE));
    mvwprintw(titlewin, 0, 0,"%s",var_barra);
    mvwprintw(titlewin, 0, tmp_borde, "%s",act_titulo_curso);
    wrefresh(titlewin);
}
void muestra_pie(int opciones[4]){    
    //según las opciones[] que traigamos mostraremos unas opciones del menú u otras
    char var_barra[max_x];

    //creamos los pares de colores
    init_pair(C_LETRA_PIE,COLOR_WHITE,COLOR_BLUE);
    
    WINDOW *footerwin; 
    footerwin=newwin(1, max_x, y_footerwin,x_footerwin);    
    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    
    wattron(footerwin,COLOR_PAIR(C_LETRA_PIE));
    mvwprintw(footerwin,0, 0,"%s",var_barra);
    
    char et_opcion0[]=ET_OPTION0;
    char et_opcion11[]=ET_OPTION11;
    char et_opcion12[]=ET_OPTION12;
    char et_opcion2[]=ET_OPTION2;
    char et_opcion3[]=ET_OPTION3;   
    int pos_opc=1;//with this variable we control the options position in the menu
// return 0;    
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
        wattron(timewin,COLOR_PAIR(C_TIMEOUT) | WA_BLINK | WA_BOLD);
    }
    mvwprintw(timewin,0, 8, "%02d:%02d",minutos,segundos);
    wrefresh(timewin);

    if(total_tiempo<MAX_TIEMPO){
        alarm(1);
    }else{
        flag_timeout=true;//se acabó el tiempo! 
        //
        wattron(childwin,WA_BLINK|COLOR_PAIR(C_LETRA_ERR));
        char tmp_cadena[]=ET_TIME_OVER;
        int tmp_borde=floor((ancho_caja-strlen(tmp_cadena))/2);
        int tmp_borde2=floor((ancho_caja-strlen(ET_PRESS_KEY_CONTINUE))/2);

        wattron(childwin,WA_BLINK | WA_BOLD);
        mvwprintw(childwin,5,tmp_borde,tmp_cadena);
        wattroff(childwin,WA_BLINK | WA_BOLD);
        mvwprintw(childwin,6, tmp_borde2, ET_PRESS_KEY_CONTINUE);
        wrefresh(childwin);
        refresh();
    }
    wattroff(timewin,COLOR_PAIR(C_TIMEOUT) | WA_BLINK | WA_BOLD);
}
void finalizar(int id_course){
    //PARAMOS EL CRONOMETRO
    WINDOW *finalwin, *descfinalwin;
    alarm(0);

    wclear(lessonwin);
    wclear(errorwin);
    wclear(timewin);
    wclear(childwin);
    wclear(keyboardwin);
    wclear(descwin);

    wrefresh(lessonwin);
    wrefresh(errorwin);
    wrefresh(timewin);
    wrefresh(childwin);
    wrefresh(keyboardwin);
//     wrefresh(descwin);

    muestra_titulo_curso(id_course);
    flag_salir=true;
    ungetch(ch);
    bool flag_opcion_valida=false;
    float minutos_reales=(float) total_tiempo/60;
    float num_ppm=(long_texto+num_errores)/minutos_reales;

    finalwin=newwin(alto_caja_final, ancho_caja_final, y_finalwin, x_finalwin);
    box(finalwin, 0, 0);   

    descfinalwin=newwin(1, ancho_caja_final, y_descfinal, x_descfinal);
    mvwprintw(descfinalwin,0, 0, ET_YOUR_SCORE);
    wrefresh(descfinalwin);
    
    mvwprintw(finalwin,2, 25, "%s",obten_titulo(id_texto,id_course));
    mvwprintw(finalwin,3, 1, "%20s",ET_PPM);
    mvwprintw(finalwin,3, 25, "%d",(int)num_ppm);
    mvwprintw(finalwin,4, 1, "%20s",ET_ERRORS);
    mvwprintw(finalwin,4, 25, "%d",num_errores);
    mvwprintw(finalwin,5, 1, "%20s",ET_TIME);
    mvwprintw(finalwin,5, 25, "%02d:%02d",minutos,segundos);
    wrefresh(finalwin);

    int tmp_opciones[4]={1,2,1,1};
    refresh();
    muestra_pie(tmp_opciones);    

   do{
        ch=getch();
        //comprobamos las opciones del footer menu
        switch(ch){
            case 27://ESC exit program
                flag_opcion_valida=true;
                flag_dentro_menus=false;
                flag_dentro_menu_lecciones=false;
                delwin(finalwin);
                delwin(descfinalwin);
                seleccionar_menu();
                return;
                break;
            case 'r':
            case 'R'://R repeat the text
                flag_opcion_valida=true;
                //do need anymore, whe are in a loop 
                break;
            case 0x10b://f3 change lesson
                flag_opcion_valida=true;
                delwin(finalwin);
                delwin(descfinalwin);

                seleccionar_menu();
                return;
                break;
            case 0x10c://f4 change course
                flag_opcion_valida=true;
                flag_dentro_menu_lecciones=false;
                delwin(finalwin);
                delwin(descfinalwin);

                seleccionar_menu();
                return;
                break;
            
        }
   }while(!flag_opcion_valida);    
}
void seleccionar_menu(){
    alarm(0);
    flag_dentro_texto=false;
    delwin(childwin);    
    endwin();    
}
int contar_195(int i_row, unsigned char todo_texto[]){
    int cont=0;
    for (int i=0; i<=i_row;++i){
        if(todo_texto[i]==195){
            cont++;
        }
    }
    return cont;
}
