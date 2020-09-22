void obten_coord_wins(){
    //X
    x_menuwin=0;    
    x_childwin=0;
    x_timewin=50;
    x_errorwin=30;
    x_finalwin=0;
    x_footerwin=0;
    x_keyboardwin=0;
    x_keybintrowin=0;
    x_titlewin=0;
    x_lessonwin=0;
    x_descwin=0;
    x_introductionwin=0;
    
    int tmp_borde=floor((max_x-ancho_caja)/2);    
    x_childwin+=tmp_borde;

    int tmp_borde_time=tmp_borde+ancho_caja-14;//14 es el tamaño del texto del tiempo    
    x_timewin=tmp_borde_time;
    
    int tmp_borde_error=floor(tmp_borde+(ancho_caja/2)-6);//6 es la mitad de lo que ocupa la caja de errores    
    x_errorwin=tmp_borde_error;

    int tmp_borde_final=floor((max_x-ancho_caja_final)/2);
    x_finalwin=tmp_borde_final;
    x_descfinal=tmp_borde_final;
    
    x_footerwin=0;
    x_keyboardwin+=tmp_borde;
    x_keybintrowin+=tmp_borde;
    
    x_titlewin=0;
    x_lessonwin+=tmp_borde;
    x_descwin+=tmp_borde;    
    int tmp_borde_menu=floor((max_x-ancho_caja_menu)/2);   
    x_menuwin+=tmp_borde_menu;
    x_descmenuwin+=tmp_borde_menu;
    //Y   
    if(max_y>31){
        y_menuwin=5;    
        y_childwin=5;
        y_timewin=2;
        y_errorwin=2;
        y_finalwin=5;
        y_footerwin=max_y-1;
        y_keyboardwin=16;
        y_keybintrowin=7;//va igual que childwin
        y_titlewin=0;
        y_lessonwin=2;
        y_descwin=4;
        y_descmenuwin=4;
        y_descfinal=4;
        y_introductionwin=3;

    }else if(max_y>=28 && max_y<=31){
        y_menuwin=4;    
        y_childwin=4;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=14;
        y_keybintrowin=8;//va igual que childwin

        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=3;
        y_descmenuwin=3;
        y_descfinal=3;
        y_introductionwin=3;

    }else if(max_y==26 || max_y==27){
        flag_muestra_borde_keyb=false;
        y_menuwin=4;    
        y_childwin=4;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=13;
        y_keybintrowin=9;//va igual que childwin
        y_titlewin=0;
        y_lessonwin=1;        
        y_descwin=3;    
        y_descmenuwin=3;
        y_descfinal=3;
        y_introductionwin=3;

        max_y_keyb=11;
    }else if(max_y==25 || max_y==24){
        flag_muestra_borde_keyb=false;
        y_menuwin=3;    
        y_childwin=3;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=11;
        y_keybintrowin=8;//va igual que childwin
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=100;//mostramos uera del rango para que no se vea
        y_descmenuwin=100;
        y_descfinal=100;
        y_introductionwin=3;

        max_y_keyb=11;        
    }else if(max_y==23){
        flag_muestra_borde_keyb=false;
        y_menuwin=2;    
        y_childwin=2;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=10;
        y_keybintrowin=8;//va igual que childwin
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=100;//mostramos uera del rango para que no se vea
        y_descmenuwin=100;
        y_descfinal=100;
        y_introductionwin=3;

        max_y_keyb=11;        
    }else if(max_y<23 && max_y>15){
        flag_muestra_borde_keyb=false;
        y_menuwin=4;    
        y_childwin=4;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=100;//mostramos uera del rango para que no se vea
        y_keybintrowin=5;//va igual que childwin
        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=3;
        y_descmenuwin=3;
        y_descfinal=3;
        y_introductionwin=2;

        max_y_keyb=11;        
    }else if(max_y<=15){
        flag_muestra_borde_keyb=false;
        y_menuwin=2;    
        y_childwin=2;
        y_timewin=1;
        y_errorwin=1;
        y_finalwin=4;
        y_footerwin=max_y-1;
        y_keyboardwin=100;//mostramos uera del rango para que no se vea
        y_keybintrowin=1;//va igual que childwin

        y_titlewin=0;
        y_lessonwin=1;
        y_descwin=100;//mostramos uera del rango para que no se vea
        y_descmenuwin=100;
        y_descfinal=100;
        max_y_keyb=11;   
        y_introductionwin=100;//mostramos uera del rango para que no se vea

        
    }
}
