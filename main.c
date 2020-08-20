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

    char var_texto[255];
    int i_row, i_col, long_var_texto;
    int tecla_leida;
    int x_child_win=0, y_child_win=3;
    int ini_w,ini_h,act_ini_w,act_ini_h,act_cur_w_actual,act_cur_h_actual;
// 
    ini_w=1;
    ini_h=4;
//     
    act_ini_w=ini_w;
    act_ini_h=ini_h;    

    pos_w_actual=act_ini_w;
    pos_h_actual=act_ini_h;    

    int width=70, height=7;

    //     //
    WINDOW *mainwin, *childwin;
// 
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
    init_pair(C_LETRA_ERR,COLOR_RED,COLOR_RED);
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
        refresh();
//         redrawwin(childwin);
    move(pos_h_actual,pos_w_actual);
    redrawwin(childwin);
// 
//  //iniciamos el bucle de lectura y comprobación de tecla pulsada
    i_row=0,i_col=0;

    while(i_row<long_texto){
        tecla_leida=leer_tecla(todo_texto[i_row],i_row);
        switch(tecla_leida){
            case 0://KO tecla incorrecta
                curs_set(0);//lo primero desactivamos el cursor pq sino se ve como se mueve y queda feo
                //mostramos el siguiente carácter en rojo y volvemos atrás
                attron(COLOR_PAIR(C_LETRA_ERR));
                pitar();
                mvprintw(pos_h_actual, pos_w_actual, " ");
                attroff(COLOR_PAIR(C_LETRA_ERR));
                move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición

                usleep(35000);//esperamos 35ms para que se pueda ver el cursor en Rojo
                redrawwin(childwin);
//                     refresh();

                if(todo_texto[i_row]!=ENTER){
                    mvprintw(pos_h_actual, pos_w_actual, "%c",todo_texto[i_row]);//Repetimos el carácter correcto
                }else{
                    mvprintw(pos_h_actual, pos_w_actual, " ");//Escribimos un espacio 
                }

                num_errores++;
                muestra_errores();
                curs_set(1);//volvemos a activar el cursor
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
                move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición
                break;
        }
    }
     /*  Clean up after ourselves  */
    delwin(childwin);
    delwin(mainwin);
    endwin();
    refresh();
    return EXIT_SUCCESS;
}
int leer_tecla(int comprueba_letra, int pos_w_actual) {
    int ch=getch();
    return (ch==comprueba_letra);
}
void muestra_errores(void){
    mvprintw(0, 30, "Fallos: %d",num_errores);
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
	segundos++;
    mvprintw(0, 50, "Tiempo: %d",segundos);
    move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición

    refresh();
    if(var_parar_crono){
        alarm(1);
    }
}
void pitar(void){
    //Más adelante configuraremos si queremos que pite o no
//     beep();
}
