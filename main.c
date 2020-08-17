#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // Para las constantes SIGALRM y similares
#include <ncurses.h>
#include <unistd.h> //incluyo este para tener la funcion sleep()
#include <string.h> //incluyo para tener strcpy()

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
#define ENTER 10
//

int main(){    
    //definimos y configuramos las variables
    char **array_texto = malloc(80 * sizeof(char));
    array_texto[0]="12345";
    array_texto[1]="12345";
    array_texto[2]="12345";
    array_texto[3]="12345";
    
    
    char tmp_texto, var_texto[255];
    
    int num_cols_texto=0, long_texto=-1;//inicializamos long_texto a -1 pq en el bucle donde la llenamos empezarermos sumando ++
    int i, i_row, i_col, long_var_texto;
    int tecla_leida;
    int x_child_win=0, y_child_win=3;
    int pos_w_inicial,pos_h_inicial, ini_w,ini_h;

    ini_w=1;
    ini_h=1;
    
    pos_w_inicial=ini_w+x_child_win;
    pos_h_inicial=ini_h+y_child_win;    

    pos_w_actual=pos_w_inicial;
    pos_h_actual=pos_h_inicial;    

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
    
    //mostramos el texto que hay que repetir
    i=0;
    while(array_texto[i]){
        long_texto++;//sumamos el salto de línea anterior aquí porque si hemos llegado aquí quiere decir que aún hay texto
        
        mvwaddstr(childwin, ini_h+i, ini_w, array_texto[i]);//imprimimos la línea
        long_texto+=strlen(array_texto[i]);//sumamos la nueva longitud del texto
        i++;
        num_cols_texto=i;//otra columna
    }
    move(pos_h_actual,pos_w_actual);
    wrefresh(childwin);

    //iniciamos el bucle de lectura y comprobación de tecla pulsada
    i_row=0,i_col=0;
    for(i_col=0;i_col<num_cols_texto;i_col++){
//     while(i<long_texto){
        strcpy(var_texto,array_texto[i_col]);
        long_var_texto=strlen(var_texto);
        
        while(i_row<=long_var_texto){
            if(var_texto[i_row]==0){
                var_texto[i_row]=ENTER;
            }
            tecla_leida=leer_tecla(var_texto[i_row],i_row);
            switch(tecla_leida){
                case 0://KO tecla incorrecta
                    //mostramos el siguiente carácter en rojo y volvemos atrás
                    curs_set(0);//lo primero desactivamos el cursor pq sino se ve como se mueve y queda feo
                    attron(COLOR_PAIR(C_LETRA_ERR));
                    pitar();
                    mvprintw(pos_h_actual, pos_w_actual, " ");
                    attroff(COLOR_PAIR(C_LETRA_ERR));
                    move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición

                    wrefresh(childwin);
                    usleep(35000);//esperamos 35ms para que se pueda ver el cursor en Rojo

                    if(var_texto[i_row]!=ENTER){
                        mvprintw(pos_h_actual, pos_w_actual, "%c",var_texto[i_row]);//Repetimos el carácter correcto
                    }else{
                        mvprintw(pos_h_actual, pos_w_actual, " ");//Escribimos un espacio 
                    }

                    num_errores++;
                    muestra_errores();
                    curs_set(1);//volvemos a activar el cursor
                    break;
                case 1://OK tecla correcta                    
                    //Si la tecla pulsada no es ENTER entonces la marcamos en negrita
                            if(var_texto[i_row]!=ENTER){
                                //Marcamos el carácter en negrita
                                attron(A_BOLD);
                                mvprintw(pos_h_actual, pos_w_actual, "%c",var_texto[i_row]);
                                attroff(A_BOLD);
                            }
                    pos_w_actual++;i_row++;//corremos una posición
                    mvprintw(pos_h_actual, pos_w_actual, "%c",var_texto[i_row]);
                    move(pos_h_actual,pos_w_actual);// colocamos el cursor en la nueva posición
                    break;
            }
        }
        i_row=0;
        pos_w_actual=pos_w_inicial;
        pos_h_actual++;
    }
//     /*  Clean up after ourselves  */
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
