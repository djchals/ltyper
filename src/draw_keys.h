#define NUM_LETRAS 46
void escribe_teclas(int id_texto,int id_course);
double array_coord_letras[NUM_LETRAS];
double array_colores_letras[NUM_LETRAS];
int array_num_letra_posicion[255];
void marca_blink_letra(int act_letra,bool flag_marcala);
void dibuja_teclado(int id_texto,int id_course);
int num_ultima_tecla=0;
int max_y_keyb=13;

void dibuja_teclado(int id_texto,int id_course){    
    int i,j;
    if(id_texto!=9998){
        keyboardwin= subwin(mainwin,max_y_keyb, 80,y_keyboardwin, x_keyboardwin);
    }else{
        keyboardwin= subwin(mainwin,max_y_keyb, 80,y_keybintrowin, x_keybintrowin);
    }

    int keyb_schetch[13][66]={
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,8,8,8,0,8,100,8,0,9,101,9,0,10,102,10,0,11,103,11,0,11,104,11,0,12,105,12,0,12,106,12,0,10,107,10,0,9,108,9,0,8,109,8,0,8,8,8,0,8,8,8,0,8,8,8,8,8,8,8,0,1,0,11,12},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,8,8,8,8,8,0,8,110,8,0,9,111,9,0,10,112,10,0,11,113,11,0,11,114,11,0,12,115,12,0,12,116,12,0,10,117,10,0,9,118,9,0,8,120,8,0,8,8,8,0,8,8,8,0,8,8,8,8,8,0,1,0,10,10},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,140,140,8,0,1},
        {1,0,8,8,8,8,8,8,0,8,121,8,0,9,122,9,0,10,123,10,0,11,124,11,0,11,125,11,0,12,126,12,0,12,127,12,0,10,128,10,0,9,129,9,0,8,130,8,0,8,8,8,0,8,8,8,0,8,8,8,8,0,1,0,9,9},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,8,8,8,8,0,8,8,8,0,8,131,8,0,9,132,9,0,10,133,10,0,11,134,11,0,11,135,11,0,12,136,12,0,12,137,12,0,10,138,10,0,9,139,9,0,8,8,8,0,8,8,8,8,8,8,8,8,8,8,0,1,0,8,8},        
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,8,8,8,8,8,0,13,13,13,13,0,13,13,13,13,0,13,13,141,141,141,141,141,141,141,141,141,141,141,141,141,141,141,141,141,141,141,13,13,0,13,13,13,13,0,13,13,13,13,0,8,8,8,8,0,8,8,8,8,0,1,0,13,13},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    mvwprintw(keyboardwin,2,67,ET_FINGER_INDEX);
    mvwprintw(keyboardwin,4,67,ET_FINGER_MIDDLE);
    mvwprintw(keyboardwin,6,67,ET_FINGER_RING);
    mvwprintw(keyboardwin,8,67,ET_FINGER_LITTLE);
    mvwprintw(keyboardwin,10,67,ET_FINGER_THUMB);
    
//  en este array quedará array_coord_letras[z]=y,x;
    array_coord_letras[0]=2.07;
    array_coord_letras[1]=2.11;
    array_coord_letras[2]=2.15;
    array_coord_letras[3]=2.19;
    array_coord_letras[4]=2.23;
    array_coord_letras[5]=2.27;
    array_coord_letras[6]=2.31;
    array_coord_letras[7]=2.35;
    array_coord_letras[8]=2.39;
    array_coord_letras[9]=2.43;
    array_coord_letras[10]=4.09;
    array_coord_letras[11]=4.13;
    array_coord_letras[12]=4.17;
    array_coord_letras[13]=4.21;
    array_coord_letras[14]=4.25;
    array_coord_letras[15]=4.29;
    array_coord_letras[16]=4.33;
    array_coord_letras[17]=4.37;
    array_coord_letras[18]=4.41;
    array_coord_letras[19]=4.45;
    array_coord_letras[20]=6.10;
    array_coord_letras[21]=6.14;
    array_coord_letras[22]=6.18;
    array_coord_letras[23]=6.22;
    array_coord_letras[24]=6.26;
    array_coord_letras[25]=6.30;
    array_coord_letras[26]=6.34;
    array_coord_letras[27]=6.38;
    array_coord_letras[28]=6.42;
    array_coord_letras[29]=6.46;
    array_coord_letras[30]=8.12;
    array_coord_letras[31]=8.16;
    array_coord_letras[32]=8.20;
    array_coord_letras[33]=8.24;
    array_coord_letras[34]=8.28;
    array_coord_letras[35]=8.32;
    array_coord_letras[36]=8.36;
    array_coord_letras[37]=8.40;
    array_coord_letras[38]=8.44;
    array_coord_letras[39]=8.48;
    array_coord_letras[40]=10.20;//32 space
    array_coord_letras[41]=5.58;//64 enter (arroba)
    array_coord_letras[42]=8.03;//126 shift left
    array_coord_letras[43]=8.52;//94 shift right
    array_coord_letras[44]=6.50;//39 tilde
    array_coord_letras[45]=6.46;//95 Ñ
 
    
    init_pair(KEYB_WHITE,COLOR_WHITE,COLOR_WHITE);
    init_pair(KEYB_BLACK,COLOR_BLACK,COLOR_BLACK);
    init_pair(KEYB_FINGER1,COLOR_WHITE,COLOR_YELLOW);
    init_pair(KEYB_FINGER2,COLOR_WHITE,COLOR_GREEN);
    init_pair(KEYB_FINGER3,COLOR_WHITE,COLOR_BLUE);
    init_pair(KEYB_FINGER4,COLOR_WHITE,COLOR_RED);
    init_pair(KEYB_FINGER5,COLOR_WHITE,COLOR_MAGENTA);    
    init_pair(KEYB_FINGER6,COLOR_WHITE,COLOR_CYAN);  
    
    //llenamos el array_colores_letras
    double tmp_coords;
    double tmp_y, tmp_x;
    int y,x;
    for(i=0;i<NUM_LETRAS;i++){
        tmp_coords=array_coord_letras[i]-0.01;

        tmp_y=floor(tmp_coords);
        tmp_x=(tmp_coords-tmp_y)*100;
        
        x=round(tmp_x);
        y=round(tmp_y);
        
        array_colores_letras[i]=keyb_schetch[y][x];
    }    
    //
    for(i=0;i<13;i++){
        for(j=0;j<66;j++){
            switch(keyb_schetch[i][j]){
                case 1:
                    if(flag_muestra_borde_keyb && id_texto!=9998){
                        wattron(keyboardwin,COLOR_PAIR(KEYB_WHITE));
                        mvwprintw(keyboardwin,i,j,"%c",35);
                    }
                    break;
                case 8:
                    wattron(keyboardwin,COLOR_PAIR(KEYB_FINGER1));
                    mvwprintw(keyboardwin,i,j,"%c",32);
                    break;
                case 9:
                    wattron(keyboardwin,COLOR_PAIR(KEYB_FINGER2));
                    mvwprintw(keyboardwin,i,j,"%c",32);
                    break;
                case 10:
                    wattron(keyboardwin,COLOR_PAIR(KEYB_FINGER3));
                    mvwprintw(keyboardwin,i,j,"%c",32);
                    break;
                case 11:
                    wattron(keyboardwin,COLOR_PAIR(KEYB_FINGER4));
                    mvwprintw(keyboardwin,i,j,"%c",32);
                    break;
                case 12:
                    wattron(keyboardwin,COLOR_PAIR(KEYB_FINGER5));
                    mvwprintw(keyboardwin,i,j,"%c",32);
                    break;
                case 13:
                    wattron(keyboardwin,COLOR_PAIR(KEYB_FINGER6));
                    mvwprintw(keyboardwin,i,j,"%c",32);
                    break;
            }
        }
    }
    //si id_texto==9998 es que estamos dibujando el teclado sin las letras, en la introducción
    escribe_teclas(id_texto,id_course);

}
void escribe_teclas(int id_texto,int id_course){
    char array_letras[NUM_LETRAS];
    char act_tecla;
    bool flag_special;
    double tmp_y, tmp_x;
    int y,x,i,j,flag_repite;
//     unsigned char tmp_special_char[2];
    //
    char act_distribucion_teclado[NUM_LETRAS]={};
    strcpy(act_distribucion_teclado,&*obten_distribucion(id_texto,id_course));
    memcpy(array_letras,act_distribucion_teclado,(strlen(act_distribucion_teclado)));
    array_letras[strlen(act_distribucion_teclado)]=0;
    //La @ equivale a ENTER
    for(i=0;i<NUM_LETRAS;i++){
        flag_special=false;
        tmp_y=floor(array_coord_letras[i]);
        tmp_x=(array_coord_letras[i]-tmp_y)*100;

        x=round(tmp_x);
        y=round(tmp_y);
        wattron(keyboardwin,COLOR_PAIR(array_colores_letras[i]));
        array_num_letra_posicion[(int) array_letras[i]]=i;

        switch(array_letras[i]){
            case 126://SHIFT LEFT
                flag_repite=1;
                act_tecla=32;
                break;
            case 94://SHIFT RIGHT
                flag_repite=8;
                act_tecla=32;
                break;
            case 64://ENTER
                flag_repite=1;
                act_tecla=32;
                break;
            case 32://SPACE
                flag_repite=18;
                act_tecla=32;
                break;
//             case 95://ñ
//                 flag_repite=0;
//                 flag_special=true;
// 
//                 tmp_special_char[0]=195;
//                 tmp_special_char[1]=177;//solo imprime ñ en minúscula
//                 tmp_special_char[2]=0x00;
                break;
            default://ANY OTHER KEY
                act_tecla=array_letras[i];
                flag_repite=0;
                break;
        }
        if(id_texto==9998){
            act_tecla=32;
        }
        for(j=0;j<(1+flag_repite);j++){
//             if(flag_special){
//                 mvwprintw(keyboardwin,y, x,"%s",tmp_special_char);
//                  wrefresh(keyboardwin);
//             }else{
                mvwprintw(keyboardwin,y, x+j,"%c",act_tecla);
//             }
        }
//         wattroff(keyboardwin,COLOR_PAIR(array_colores_letras[i]));
    }   
}
void marca_blink_letra(int tmp_letra,bool flag_marcala){
    char act_tecla;//necesario para marcar los espacios y el enter
    int j,flag_repite;
//     bool flag_special=false;

//     unsigned char tmp_special_char[2];
//     *(tmp_special_char+0)=195;
//     tmp_special_char[2]=0x00;
    //
   
    //veriicamos si es un caracter especial
//     switch(tmp_letra){
//         case 161://á
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=97;//a
//             break;
//         case 129://Á
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=65;//A
//             break;
//         case 169://é
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=101;//e
//             break;
//         case 137://É
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=69;//E
//             break;
//         case 173://í
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=105;//i
//             break;
//         case 141://Í
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=73;//I
//             break;
//         case 179://ó
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=111;//o
//             break;
//         case 147://Ó
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=79;//O
//             break;
//         case 186://ú
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=117;
//             break;
//         case 154://Ú
//             marca_blink_letra(39,flag_marcala);//marcamos la tilde ya que es un carácter acentuado
//             tmp_letra=85;//U
//             break;
// //             ALTA ACABAR ESTA PARTE
// //         case 164://ñ 
// //             tmp_letra=95;//U
// //             break;
// //         case 165://Ñ
// //             marca_blink_letra(126,flag_marcala);//con las letras que pulsamos con mano der marcamos SHIFT izq
// //             tmp_letra=95;//U
//             break;
//     }
    int act_posicion=array_num_letra_posicion[toupper(tmp_letra)];

    double tmp_y=floor(array_coord_letras[act_posicion]);
    double tmp_x=(array_coord_letras[act_posicion]-tmp_y)*100;
    int x=round(tmp_x);
    int y=round(tmp_y);
    if(isupper(tmp_letra)){
        switch(act_posicion){
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 20:                
            case 21:
            case 22:
            case 23:
            case 24:
            case 30:                
            case 31:
            case 32:
            case 33:
            case 34:
                marca_blink_letra(94,flag_marcala);//con las letras que pulsamos con mano izq marcamos SHIFT der
                break;
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:                
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 35:
            case 36:
            case 37:
            case 38:
            case 39:
            case 40:
                marca_blink_letra(126,flag_marcala);//con las letras que pulsamos con mano der marcamos SHIFT izq
                break;
        }
    }
    if(flag_marcala){//la vamos a marcar
        wattron(keyboardwin,WA_BLINK | WA_BOLD | COLOR_PAIR(array_colores_letras[act_posicion]));
    }else{//la desmarcamos
        wattroff(keyboardwin,WA_BLINK | WA_BOLD);
        wattron(keyboardwin,COLOR_PAIR(array_colores_letras[act_posicion]));
    }
    switch(toupper(tmp_letra)){
//         ALTA ACABAR ESTA PARTE
//         case 95://ñ o Ñ
//             flag_repite=0;
//             if(flag_marcala){
//                 flag_special=true;
//                 tmp_special_char[1]=177;
//             }else{
//                 act_tecla=32;
//             }
//             break;
        case 126://SHIFT LEFT
            flag_repite=1;
            act_tecla=(flag_marcala?124:32);
            break;
        case 94://SHIFT RIGHT
            flag_repite=8;
            act_tecla=(flag_marcala?124:32);
            break;
        case 64:
            flag_repite=1;
            act_tecla=(flag_marcala?124:32);
            break;
        case 32:
            flag_repite=18;
            act_tecla=(flag_marcala?124:32);
            break;
        default:
            act_tecla=tmp_letra;
            flag_repite=0;
            break;   
    }
    
//     if(flag_special){
//         mvwprintw(keyboardwin,y, x,"%s",tmp_special_char);
//     }else{
        for(j=0;j<(1+flag_repite);j++){
            mvwprintw(keyboardwin,y, x+j,"%c",toupper(act_tecla));
        }
//     }
 wrefresh(keyboardwin);
}
