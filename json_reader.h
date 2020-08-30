char *obten_texto(int id_texto);
char *obten_titulo(int id_texto);
int obten_num_titulos();
void obten_titulos();
void quita_espacios(char *act_texto);
json_object *parsed_json, *titulos;
void _init_json();

void _init_json(){
    FILE *fp;
	char buffer[1024];
    
    fp = fopen("db_texts.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

    parsed_json = json_tokener_parse(buffer);
}
int obten_num_titulos(){
    json_object_object_get_ex(parsed_json, "titulos", &titulos);
    return json_object_array_length(titulos);
}
char *obten_texto(int id_texto){
    json_object *textos, *texto;

    json_object_object_get_ex(parsed_json, "textos", &textos);
    texto = json_object_array_get_idx(textos, id_texto);
    char *act_texto=(char *) json_object_get_string(texto);
    quita_espacios(act_texto);
    return act_texto;
}
void quita_espacios(char *act_texto){
    int long_texto, i=0, i_row=0;
    long_texto=strlen(act_texto);
    unsigned char tmp_cadena[255];
    bool flag_sumar=false;
    //quitamos los espacios de sobra
    for(i=0;i<long_texto;i++){        
        flag_sumar=true;
        if(act_texto[i]!=0x0a && isblank(act_texto[i]) && isblank(act_texto[i+1])) { 
            flag_sumar=false;
        }
        if(act_texto[i-1]==0x0a && (isblank(act_texto[i]) || isspace(act_texto[i]))) {
            flag_sumar=false;
        }
        if((isblank(act_texto[i]) || isspace(act_texto[i])) && act_texto[i+1]==0x0a) {
            flag_sumar=false;
        }

        if(flag_sumar){
            tmp_cadena[i_row]=act_texto[i];
//             printf("tmp_cadena[i_row] (%d)\n",tmp_cadena[i_row]);
            i_row++;
        }
    }    
    tmp_cadena[i_row]=0;//hacemos esto para saber que aquí acaba la cadena
    strcpy(act_texto, tmp_cadena);//en la dirección del puntero de act_texto escribimos tmp_cadena
}

char *obten_titulo(int id_texto){
    json_object *titulo;
    titulo=json_object_array_get_idx(titulos, id_texto);

    char *act_titulo=(char *) json_object_get_string(titulo);
    quita_espacios(act_titulo);
    return act_titulo;
}
char *trim(char *s){
    char *start = s;
    /* Nos comemos los espacios al inicio */
    while(*start && isspace(*start)){
        ++start;
    }
    char *k = start;
    char *end = start;

    /* Nos comemos los espacios al final */
    while(*k) {
        if(!isspace(*(k++)) )
        end = k;
    }

    /* Escribimos el terminados */
    *end = 0;
    return start;
}
