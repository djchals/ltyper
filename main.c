#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // Para las constantes SIGALRM y similares
#include <ncurses.h>
#include <unistd.h> //incluyo este para tener la funcion sleep()

//PROTOTIPOS y VARIABLES GLOBALES
int leer_tecla(int comprueba_letra, int pos_w_actual);
int num_errores;
void muestra_errores(void);
void muestra_cabecera(void);
void contar_segundos();
void pitar(void);
int segundos=-1;
int j=0;
bool var_parar_crono=true;
int pos_w_actual=0, pos_h_actual=0;

//DEFINIMOS LOS COLORES QUE VAMOS A USAR
#define C_LETRA_ERR    1
#define C_LETRA_CUR    2
#define C_TEXTO        3
//

int main(){    
    //definimos y configuramos las variables
    char var_texto[]={"hola que tal, hola nhola hola"};
    int long_texto=(sizeof(var_texto)/sizeof(char))-1;
    int i;
    int tecla_leida, ini_w_texto,ini_h_texto;
    int x_child_win=0, y_child_win=3;
    
    ini_w_texto=1;
    ini_h_texto=1;
    
    pos_w_actual=ini_w_texto+x_child_win;
    pos_h_actual=ini_h_texto+y_child_win;    

    int width=70, height=7;
    int rows=25, cols=80;
    //
    WINDOW *mainwin, *childwin;

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

    //Creamos la ventana
    childwin = subwin(mainwin, height, width, y_child_win, x_child_win);
    box(childwin, 0, 0);
    
    //iniciamos el cronometro asociando la señal SIGALRM a la función contar_segundos
	signal(SIGALRM, contar_segundos);
    muestra_cabecera();
    
    //para cambiar color del texto, hay que descomentar esto:
    mvwaddstr(childwin, ini_h_texto, ini_w_texto, var_texto);
    move(pos_h_actual,pos_w_actual);

    //iniciamos el bucle de lectura y comprobación de tecla pulsada
    i=0;
    wrefresh(childwin);//
    while(i<long_texto){

        tecla_leida=leer_tecla(var_texto[i],i);
        switch(tecla_leida){
            case 0://KO tecla incorrecta
                //mostramos el siguiente carácter en rojo y volvemos atrás
                curs_set(0);//lo primero desactivamos el cursor pq sino se ve como se mueve y queda feo
                attron(COLOR_PAIR(C_LETRA_ERR));
                pitar();
                mvprintw(pos_h_actual, pos_w_actual, "#");
                attroff(COLOR_PAIR(C_LETRA_ERR));
                move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición

                wrefresh(childwin);
                 usleep(100000);//esperamos 100ms para que se pueda ver el cursor en Rojo

                mvprintw(pos_h_actual, pos_w_actual, "%c",var_texto[i]);

                num_errores++;
                muestra_errores();
                move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición
                curs_set(1);//volvemos a activar el cursor
                refresh();
                break;
            case 1://OK tecla correcta                
                //Marcamos el carácter en negrita
                attron(A_BOLD);
                mvprintw(pos_h_actual, pos_w_actual, "%c",var_texto[i]);
                attroff(A_BOLD);
                wrefresh(childwin);//refrescamos la ventana para que se vea el cambio
                //
                
                pos_w_actual++;i++;//corremos una posición
                mvprintw(pos_h_actual, pos_w_actual, "%c",var_texto[i]);
                move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición
                wrefresh(childwin);//actualizamos la ventana para poder ver los cambios
                break;
            case 2://EXIT, de momento aquí no llegamos nunca
                mvprintw(12, pos_w_actual, "quiero salir %d",tecla_leida);
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
