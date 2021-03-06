#include <dirent.h>

char *obten_texto(int id_texto,int id_course);
char *obten_titulo(int id_texto, int id_course);
char *obten_course_title(int id_course);
char *obten_distribucion(int id_texto, int id_course);
char *obten_texto_especial(int id_texto,int id_course);
void obten_titulos();
int obten_num_titulos(int id_course);
void quita_espacios(char *act_texto);

json_object *parsed_json, *titulos;

void proceso_archivo(char *archivo);/* Función que hace algo con un archivo */
int obten_id_course();
int _init_cursos();

json_object *array_json_parseds[MAX_COURSES];
json_object *tmp_json_parsed;
char ruta[]=DATADIR;
int array_cursos[MAX_COURSES];//array_cursos[i]=id_course
int num_cursos=0;

void quita_espacios(char *act_texto){
    int long_texto, i=0, i_row=0;
    long_texto=strlen(act_texto);
    char tmp_cadena[1024];
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
        if((isblank(act_texto[i]) || isspace(act_texto[i])) && (act_texto[i+1]==0x0a || act_texto[i+1]==0x00)) {
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
int _init_cursos(){
    srand (getpid());
    DIR *dir;/* Con un puntero a DIR abriremos el directorio */
    struct dirent *ent;/* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    dir=opendir(ruta);//Empezaremos a leer en el directorio db
    int tmp_id;
    
    /* Miramos que no haya error */
    if(dir==NULL){
        char tmp_not_open[100];
        sprintf(tmp_not_open,"%s \"%s\"",ET_CANNOT_OPEN_DIR,ruta);
        perror(tmp_not_open);
        exit(EXIT_FAILURE);
    }
    while((ent=readdir(dir))!=NULL){/* Leyendo uno a uno todos los archivos que hay */
        /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
            /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
            proceso_archivo(ent->d_name);
            tmp_id=obten_id_course();
            array_cursos[tmp_id-1]=tmp_id;
            num_cursos++;
        }
    }
    closedir (dir);
    return EXIT_SUCCESS;
} 

void proceso_archivo(char *archivo){
    char *ruta_archivo = malloc(strlen(ruta) + strlen(archivo) + 1); // +1 for the null-terminator
    strcpy(ruta_archivo, ruta);
    strcat(ruta_archivo, archivo);
    
    FILE *fp;
	char buffer[34000];//si el archivo ocupase más de 10kb habria que aumentar este valor
    fp = fopen(ruta_archivo,"r");
	int flag_archivo_leido=fread(buffer, 34000, 1, fp);//si el archivo ocupase más de 10kb habria que aumentar este valor
	fclose(fp);
    tmp_json_parsed = json_tokener_parse(buffer); 
    
    int id_course=obten_id_course();
    array_json_parseds[id_course]=*&tmp_json_parsed;
}
int obten_num_titulos(int id_course){
    json_object_object_get_ex(array_json_parseds[id_course], "titles", &titulos);
    return json_object_array_length(titulos);
}
char *obten_texto(int id_texto, int id_course){
    json_object *textos, *texto;

    json_object_object_get_ex(array_json_parseds[id_course], "texts", &textos);
    texto = json_object_array_get_idx(textos, id_texto);
    char *act_texto=(char *) json_object_get_string(texto);
    quita_espacios(act_texto);
    return act_texto;
}
char *obten_titulo(int id_texto, int id_course){
    json_object *titulo;
    
    json_object_object_get_ex(array_json_parseds[id_course], "titles", &titulos);
    titulo=json_object_array_get_idx(titulos, id_texto);

    char *act_titulo=(char *) json_object_get_string(titulo);
    quita_espacios(act_titulo);
    return act_titulo;
}
char *obten_distribucion(int id_texto, int id_course){
    json_object *distribution;
    json_object_object_get_ex(array_json_parseds[id_course], "distribution", &distribution);
//     char *tmp_distribution=(char *)json_object_get_string(distribution);
    return (char *)json_object_get_string(distribution);
}

int obten_id_course(){
    json_object *id_course;
    json_object_object_get_ex(tmp_json_parsed, "id_course", &id_course);
    int *tmp_id=(int *) json_object_get_int(id_course);
    return (int)tmp_id;
}
char *obten_course_title(int id_course){
//     char *act_titulo=array_et_course_title[id_course];
//     return act_titulo;
    return (char *) array_et_course_title[id_course];
    
}
char *obten_texto_especial(int id_texto,int id_course){
    int array_longs[]={125,190,255,320,385,440};
    long_texto=array_longs[id_texto];
    
    //long_texto funcionará muy bien con los tamaños: 125 190 255 320

    char array_imprimibles[200];
    switch(id_course){
        case 9:
            strcpy(array_imprimibles,"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz?!-'.,1234567890");
            break;
        case 10:
            strcpy(array_imprimibles,"!'#$%^&*()-={}][<>/-_+|");
            break;
    }
    int num_imprimibles=strlen(array_imprimibles)-1;//esto es así y punto, porque si pongo justo da error
    int num_espacios[100];
    int numero, i, i_espacio, i_enter;
    char texto_random[long_texto];
    int j=0;//con este contador contamos cuantos caracteres llevamos escritos seguidos para ver si ya toca escribir un espacio o no

    for(i=0;i<100;i++){
        numero=rand()%7+2;
        num_espacios[i]=numero;
    }

    i_espacio=0;i_enter=0;
    for(i=0;i<long_texto;i++){
        
        numero=rand()%num_imprimibles+1;
        
        if(rand()%30+1==2 && isalpha(array_imprimibles[numero])){
            texto_random[i]=toupper(array_imprimibles[numero]);
        }else{
            texto_random[i]=array_imprimibles[numero];
        }
        
        j++;
        i_enter++;
        if(i!=long_texto && i_enter==65){
            texto_random[i]=10;
            i_enter=0;
            j=0;
        }
        
        //comprobamos si hemos de colocar un espacio en este momento
        if(j==num_espacios[i_espacio] && i!=long_texto){
            texto_random[i]=32;
            i_espacio++;
            j=0;
        }
    }
    texto_random[i]=0;

    char *tmp_return;
    tmp_return=malloc(sizeof(char)*strlen(texto_random));
    memcpy((void *)tmp_return,texto_random,strlen(texto_random));
    return tmp_return;
}
