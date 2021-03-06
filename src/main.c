#include "header.h"
#include "json_reader.h"
#include "menus.h"
#include "draw_keys.h"
#include "coord_wins.h"
#include "introduction.h"

int main(){
    _init_cursos();//necesario para leer el archivo json en json_reader.h
    _init_ncurses();//it s not the same
    obten_coord_wins();
    bucle_menus();
    printf("\n\%s\n\n",ET_GOOD_BYE);
    return 0;
}
void bucle_menus(){
    flag_dentro_menus=true;
    int opcion_selec;
    if(flag_primera_vez){ //Solo mostraremos la introducción al iniciar el programa
        muestra_presentacion();
        flag_primera_vez=false;
    }    
    while(flag_dentro_menus){
        flag_dentro_menu_lecciones=true;
        opcion_selec=0;
        leccion_sel=0;
        opcion_selec=muestra_menu(0);
        while(flag_dentro_menu_lecciones && opcion_selec!=9999){
            //si estamos en el curso de los 100 textos omitimos muestra_menu(lecciones)
            if(opcion_selec!=8){
                leccion_sel=muestra_menu(array_cursos[opcion_selec]);//aquí ya deiniremos id_course
            }else if(leccion_sel==0){
                leccion_sel=rand()%101;
            }
            flag_dentro_texto=true;//con este flag controlamos si estamos escribiendo un texto y lo estamos repitiendo
            clear();
            
            while(flag_dentro_texto && leccion_sel!=9999){
                muestra_texto(leccion_sel,array_cursos[opcion_selec]);
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
    ini_w=1;
    ini_h=1;
     
    act_ini_w=ini_w;
    act_ini_h=ini_h;    

    pos_w_actual=act_ini_w;
    pos_h_actual=act_ini_h;   
    //

    //obtenemos el texto que hay que repetir
    char *array_puntero_texto;
    bool flag_blink;
    switch(id_course){
        case 10:
            array_puntero_texto=obten_texto_especial(act_id_texto,9);
            flag_blink=false;
            break;
        case 11:
            array_puntero_texto=obten_texto_especial(act_id_texto,10);
            flag_blink=false;
            break;    
        case 9:
            array_puntero_texto=obten_texto(act_id_texto,id_course);//id_texto es una variable global
            flag_blink=false;//es un texto random así que no marcaremos las letras
            break;
        default:
            array_puntero_texto=obten_texto(act_id_texto,id_course);//id_texto es una variable global
            flag_blink=true;
            break;
    }

    long_texto=strlen(array_puntero_texto);
    unsigned char todo_texto[long_texto];
    memcpy(todo_texto,array_puntero_texto,long_texto);
    array_puntero_texto[long_texto]=0;//porsiaca.com añadimos el final del string
    //

    //preparamos el terminal para el modo menú/lectura de tecla
    clear();
    noecho();/*  Turn off key echoing */
    keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */  
    curs_set(0);//Ocultamos el cursor (ahora lo estoy mostrando para ver los errores)
    // 

    //iniciamos el cronometro asociando la señal SIGALRM a la función contar_segundos
	signal(SIGALRM, contar_segundos);
    
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado
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
    //dibujamos el teclado salvo que sea un texto con caracteres aleatorios
//     if(id_course!=9 && id_course!=10 && id_course!=9){
        dibuja_teclado(id_texto,id_course);   
//     }
    //
    refresh();
    //iniciamos el bucle de lectura y comprobación de tecla pulsada

    muestra_cabecera(id_texto,id_course);//esta línea debe ir despues del refresh();
    int tmp_opciones[5]={1,0,1,1,1};
    muestra_pie(tmp_opciones);//esta línea debe ir despues del refresh();
    
    //esto será necesario para el tratamiento de los carácteres especiales
//     unsigned char tmp_special_char[2];
//     *(tmp_special_char+0)=195;
//     tmp_special_char[2]=0x00;
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
            (flag_blink)?marca_blink_letra(64,true):1;
        }else if(isalnum(todo_texto[i_row]) || ispunct(todo_texto[i_row]) || isspace(todo_texto[i_row])){
            (flag_blink)?marca_blink_letra(todo_texto[i_row],true):1; 
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
                endwin();
                return;
                break;
            case 0x109:/*f1 help*/break;
            case 0x10a:/*f2 cancel*/
                long_texto=i_row;
                finalizar(id_course,true);
                return;//lo frenamos aquí en seco, para que no se vaya al final de este while y ejecute otra vez finalizar();
                break;
            case 0x10b:/*f3 change lesson*/
                //si es 100textos no habrá menú lecciones
                if(id_course==9){
                    leccion_sel=0;
                }
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
                    (flag_blink)?marca_blink_letra(todo_texto[i_row],false):1; //desmarcamos la última tecla pulsada
                    actualiza_cursor(i_row,pos_h_actual,pos_w_actual,act_attrs,todo_texto);
                    pos_w_actual++;//corremos una posición
                    
                }else{
                    //desmarcamos la última tecla pulsada
                    (flag_blink)?marca_blink_letra(64,false):1;
                    //
                    actualiza_cursor(i_row,pos_h_actual,pos_w_actual,act_attrs,todo_texto);
                    pos_w_actual=ini_w;
                    pos_h_actual++;
                }
                i_row++;
                break;
        }
        wrefresh(childwin);
    }

    finalizar(id_course,false);
}
// bool is_special(int tmp_caracter){
//     switch(tmp_caracter){
//         case 161://á
//         case 169://é
//         case 173://í
//         case 179://ó
//         case 186://ú
//         case 129://Á
//         case 137://É
//         case 141://Í
//         case 147://Ó
//         case 154://Ú
//         case 177://ñ
//         case 145://Ñ
//             return true;
//         break;
//         default:
//             return false;
//         break;
//     }
// }
int comprueba_tecla(int ch, int i_row, unsigned char todo_texto[]){
//      if(ch<=128){
//          return (int)(ch==todo_texto[i_row]);
//     }
//     switch(ch){
//         case 161://á
//         case 169://é
//         case 173://í
//         case 179://ó
//         case 186://ú
//         case 129://Á
//         case 137://É
//         case 141://Í
//         case 147://Ó
//         case 154://Ú
//         case 177://ñ
//         case 145://Ñ
    if(ch==32 && todo_texto[i_row]==ENTER){
        return true;
    }else{
        return (int)(ch==todo_texto[i_row]);
    }
//             break;
//         default:
//             return 0;
//             break;
//     }
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
    }else if(isprint(todo_texto[i_row])){
        mvwprintw(childwin,pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);
    }else{
        //AQUÍ LOS CARACTERES ESPECIALES
//         if(is_special(todo_texto[i_row])){
//             unsigned char tmp_special_char[2];
//             *(tmp_special_char+0)=195;
//             *(tmp_special_char+1)=todo_texto[i_row];
//             mvwprintw(childwin,pos_h_actual, pos_w_actual, "%s",tmp_special_char);
//         }
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
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado

    //Creamos las ventana de tiempo y errores ya que se tendrán que ir rerescando periodicamente
    timewin=newwin(1, 15, y_timewin, x_timewin);
    errorwin=newwin(1, 15, y_errorwin, x_errorwin);
    lessonwin=newwin(1, max_x, y_lessonwin,x_lessonwin);  
    descwin=newwin(1, max_x, y_descwin,x_descwin);  
    //
    
    muestra_titulo_curso(id_course);
    if(id_course==9){
        mvwprintw(lessonwin,0, 0,"%s",ET_RANDOM_PARAGRAPH);
    }else if(id_course==10 || id_course==11){
        mvwprintw(lessonwin,0, 0,"%s %s",obten_titulo(id_texto,id_course),ET_RANDOM_CHARS);        
    }else{
        mvwprintw(lessonwin,0, 0,"%s %s",ET_LESSON, obten_titulo(id_texto,id_course));
    }
    wrefresh(lessonwin);

    mvwprintw(descwin,0,0, ET_DESC);
    wrefresh(descwin);

    muestra_errores();

    contar_segundos();
    wrefresh(timewin);
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
void muestra_pie(int opciones[5]){    
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado

    //según las opciones[] que traigamos mostraremos unas opciones del menú u otras
    char var_barra[max_x];

    //creamos los pares de colores
    init_pair(C_LETRA_PIE,COLOR_WHITE,COLOR_BLUE);
    
    footerwin=newwin(1, max_x, y_footerwin,x_footerwin);    
    memset(var_barra,32,max_x);//llenamos var_barra con espacios para mostrar el fondo del pie
    
    wattron(footerwin,COLOR_PAIR(C_LETRA_PIE));
    mvwprintw(footerwin,0, 0,"%s",var_barra);
    
    int pos_opc=1;//with this variable we control the options position in the menu
    
    if(opciones[0]==1){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"ESC");
        pos_opc=pos_opc+3;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0, pos_opc,ET_OPTION0);
        pos_opc=pos_opc+strlen(ET_OPTION0)+3;
    }else if(opciones[0]==2){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,ET_PRESS_ANY_KEY);
        wattroff(footerwin,WA_BOLD);
        pos_opc=pos_opc+strlen(ET_PRESS_ANY_KEY);        
    }
    if(opciones[1]==1){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"F1");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0, pos_opc,ET_OPTION1);
        pos_opc=pos_opc+strlen(ET_OPTION1)+3;
    }
    if(opciones[2]==1){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"F2");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0, pos_opc,ET_OPTION21);
        pos_opc=pos_opc+strlen(ET_OPTION21)+3;
    }else if(opciones[2]==2){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"F2");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0, pos_opc,ET_OPTION22);
        pos_opc=pos_opc+strlen(ET_OPTION22)+3;        
    }
    if(opciones[3]){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0,pos_opc,"F3");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0,pos_opc,ET_OPTION3);
        pos_opc=pos_opc+strlen(ET_OPTION3)+3;
    }
    if(opciones[4]){
        wattron(footerwin,WA_BOLD);
        mvwprintw(footerwin,0,pos_opc,"F4");
        pos_opc=pos_opc+2;
        wattroff(footerwin,WA_BOLD);
        mvwprintw(footerwin,0, pos_opc,"=");
        pos_opc++;        
        mvwprintw(footerwin,0,pos_opc,ET_OPTION4);
        pos_opc=pos_opc+strlen(ET_OPTION4)+3;
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
void finalizar(int id_course,bool flag_cancela_texto){
    obten_coord_wins();//volvemos a obtener las coordenadas por si acaso ha redimensionado

    //PARAMOS EL CRONOMETRO
    WINDOW *finalwin, *descfinalwin, *frasefinalwin;
    int tmp_opciones[5];
    alarm(0);
    
    wclear(lessonwin);
    wclear(errorwin);
    wclear(timewin);
    wclear(childwin);
    wclear(keyboardwin);
    wclear(descwin);
    wclear(footerwin);
    
    wrefresh(descwin);
    wrefresh(lessonwin);
    wrefresh(errorwin);
    wrefresh(timewin);
    wrefresh(childwin);
    wrefresh(keyboardwin);
    wrefresh(footerwin);
    
    muestra_titulo_curso(id_course);
    flag_salir=true;

    bool flag_opcion_valida=false;
    float minutos_reales=(float) total_tiempo/60;
    //si se cancela el texto en el segundo 0 (demasiado rápido) num_ppm=0, sino se calcula
    float num_ppm=(!(flag_cancela_texto && minutos_reales==0)?(long_texto+num_errores)/minutos_reales:0);
    float porcentaje_errores=(long_texto+num_errores!=0)?((float) num_errores/(float) (long_texto+num_errores))*100:0;

    finalwin=newwin(alto_caja_final, ancho_caja_final, y_finalwin, x_finalwin);
    box(finalwin, 0, 0);   

    //imprimimos descripción del finalwin
    descfinalwin=newwin(1, max_x, y_descfinalwin, 0);
    mvwprintw(descfinalwin,0,floor((max_x-strlen(ET_YOUR_SCORE))/2),ET_YOUR_SCORE);
    wrefresh(descfinalwin);
    //
    char tmp_et_lesson[50];
    if(id_course==9){
        strcpy(tmp_et_lesson,ET_RANDOM_PARAGRAPH);
    }else if(id_course==10 || id_course==11){
        strcpy(tmp_et_lesson,ET_RANDOM_CHARS);        
    }else{
        strcpy(tmp_et_lesson,ET_LESSON);
    }
    
    mvwprintw(finalwin,2, 1, "%20s",tmp_et_lesson); 
    if(id_course!=9){
        mvwprintw(finalwin,2, 25, "%s",obten_titulo(id_texto,id_course));
    }
    mvwprintw(finalwin,3, 1, "%20s",ET_PPM);
    mvwprintw(finalwin,3, 25, "%d",(int)num_ppm);
    mvwprintw(finalwin,4, 1, "%20s",ET_ERRORS);
    mvwprintw(finalwin,4, 25, "%d (%.2f%)",num_errores,porcentaje_errores);
    mvwprintw(finalwin,5, 1, "%20s",ET_TIME);
    mvwprintw(finalwin,5, 25, "%02d:%02d",minutos,segundos);
    wrefresh(finalwin);

    //imprimimos la frase final centradita
    frasefinalwin=newwin(10, max_x, y_frasefinalwin, 0);
    if(!flag_cancela_texto){
        if(num_errores==0 && long_texto>0){
            mvwprintw(frasefinalwin, 0, ((strlen(ET_FINISH_PHRASE11)<max_x)?floor((max_x-strlen(ET_FINISH_PHRASE11))/2):0),ET_FINISH_PHRASE11);
            mvwprintw(frasefinalwin, 2, ((strlen(ET_FINISH_PHRASE12)<max_x)?floor((max_x-strlen(ET_FINISH_PHRASE12))/2):0),ET_FINISH_PHRASE12);
            mvwprintw(frasefinalwin, 3, ((strlen(ET_FINISH_PHRASE13)<max_x)?floor((max_x-strlen(ET_FINISH_PHRASE13))/2):0),ET_FINISH_PHRASE13);
            tmp_opciones[0]=1;
            tmp_opciones[1]=0;
            tmp_opciones[2]=0;
            tmp_opciones[3]=1;
            tmp_opciones[4]=1;
        }else if(num_errores>0 && num_errores<=3 && long_texto>0){
            mvwprintw(frasefinalwin, 0, ((strlen(ET_FINISH_PHRASE21)<max_x)?floor((max_x-strlen(ET_FINISH_PHRASE21))/2):0),ET_FINISH_PHRASE21);
            mvwprintw(frasefinalwin, 1, ((strlen(ET_FINISH_PHRASE22)<max_x)?floor((max_x-strlen(ET_FINISH_PHRASE22))/2):0),ET_FINISH_PHRASE22);
            tmp_opciones[0]=1;
            tmp_opciones[1]=0;
            tmp_opciones[2]=0;
            tmp_opciones[3]=1;
            tmp_opciones[4]=1;
        }else if(long_texto>0){
            mvwprintw(frasefinalwin, 0, ((strlen(ET_FINISH_PHRASE3)<max_x)?floor((max_x-strlen(ET_FINISH_PHRASE3))/2):0),ET_FINISH_PHRASE3,num_errores);
            tmp_opciones[0]=1;
            tmp_opciones[1]=0;
            tmp_opciones[2]=2;
            tmp_opciones[3]=1;
            tmp_opciones[4]=1;
        }
    }else{
        wattron(frasefinalwin,COLOR_PAIR(C_LETRA_ERR) | WA_BOLD);
        mvwprintw(frasefinalwin, 0, ((strlen(ET_FINISH_PHRASE_CANCELLED)<max_x)?floor((max_x-strlen(ET_FINISH_PHRASE_CANCELLED))/2):0),ET_FINISH_PHRASE_CANCELLED);
        wattroff(frasefinalwin,COLOR_PAIR(C_LETRA_ERR) | WA_BOLD);
        tmp_opciones[0]=1;
        tmp_opciones[1]=0;
        tmp_opciones[2]=2;
        tmp_opciones[3]=1;
        tmp_opciones[4]=1;    
    }
    wrefresh(frasefinalwin);
    //
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
                endwin();
                return;
                break;
            case 0x10a:/*f2 repeat the text*/
                if(tmp_opciones[2]==2){
                    if(id_course==9){
                        leccion_sel=id_texto;
                    }
                    flag_opcion_valida=true;
                }
                //do need anymore, whe are in a loop 
                break;
            case 0x10b://f3 change lesson
                //si es 100textos no habrá menú lecciones
                if(id_course==9){
                    leccion_sel=0;
                }
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
}
