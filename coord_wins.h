void obten_coord_wins(){
    //X
    x_childwin=0;
    x_timewin=50;
    x_errorwin=30;
    x_finalwin=0;
    x_footerwin=0;
    x_keyboardwin=0;
    x_titlewin=0;
    x_lessonwin=0;
    x_descwin=0;    

    int tmp_borde=floor((max_x-ancho_caja)/2);    
    x_childwin+=tmp_borde;

    int tmp_borde_time=tmp_borde+ancho_caja-14;//14 es el tamaño del texto del tiempo    
    x_timewin=tmp_borde_time;
    
     int tmp_borde_error=tmp_borde+(ancho_caja/2)-6;//6 es la mitad de lo que ocupa la caja de errores    
    x_errorwin=tmp_borde_error;
    
    x_finalwin=0;
    x_footerwin=0;
    x_keyboardwin+=tmp_borde;
    x_titlewin=0;
    x_lessonwin+=tmp_borde;
    x_descwin+=tmp_borde;    
    
    //Y   
    if(max_y>31){
        y_childwin=5;
        y_timewin=2;
        y_errorwin=2;
        y_finalwin=5;
        y_footerwin=max_y-1;
        y_keyboardwin=16;
        y_titlewin=0;
        y_lessonwin=2;
        y_descwin=4;
    }else if(max_y>=28 && max_y<=31){
        y_childwin=4;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=14;
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=3;
    }else if(max_y==26 || max_y==27){
        flag_muestra_borde_keyb=false;
        y_childwin=4;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=13;
        y_titlewin=0;
        y_lessonwin=1;        
        y_descwin=3;        
        max_y_keyb=11;
    }else if(max_y==25 || max_y==24){
        flag_muestra_borde_keyb=false;
        y_childwin=3;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=11;
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=100;//mostramos uera del rango para que no se vea
        max_y_keyb=11;        
    }else if(max_y==23){
        flag_muestra_borde_keyb=false;
        y_childwin=2;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=10;
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=100;//mostramos uera del rango para que no se vea
        max_y_keyb=11;        
    }else if(max_y<23 && max_y>15){
        flag_muestra_borde_keyb=false;
        y_childwin=4;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=100;//mostramos uera del rango para que no se vea
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=3;
        max_y_keyb=11;        
    }else if(max_y<=15){
        flag_muestra_borde_keyb=false;
        y_childwin=2;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=100;//mostramos uera del rango para que no se vea
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=100;//mostramos uera del rango para que no se vea
        max_y_keyb=11;   
    }
}
