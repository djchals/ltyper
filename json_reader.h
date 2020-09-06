#include <sys/types.h>
#include <dirent.h>

char *obten_texto(int id_texto,int id_course);
char *obten_titulo(int id_texto, int id_course);
char *obten_course_title(int id_course);
int obten_num_titulos(int id_course);
void obten_titulos();
void quita_espacios(char *act_texto);
json_object *parsed_json, *titulos;

void error(const char *s);/* Función para devolver un error en caso de que ocurra */
void proceso_archivo(char *archivo);/* Función que hace algo con un archivo */
int obten_id_course();
int _init_cursos();

json_object *array_json_parseds[MAX_COURSES];
json_object *tmp_json_parsed;

char ruta[]="db/";
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
    DIR *dir;/* Con un puntero a DIR abriremos el directorio */
    struct dirent *ent;/* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    dir=opendir(ruta);//Empezaremos a leer en el directorio db
    /* Miramos que no haya error */
    if(dir==NULL){
        error("No puedo abrir el directorio");
    }
    while((ent=readdir(dir))!=NULL){/* Leyendo uno a uno todos los archivos que hay */
        /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
            /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
            proceso_archivo(ent->d_name);
            array_cursos[num_cursos]=obten_id_course();
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
	char buffer[1024];
    fp = fopen(ruta_archivo,"r");
	fread(buffer, 1024, 1, fp);
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
int obten_id_course(){
    struct json_object *id_course;
    json_object_object_get_ex(tmp_json_parsed, "id_course", &id_course);
    int *tmp_id=(int *) json_object_get_int(id_course);
    return (int)tmp_id;
}
char *obten_course_title(int id_course){
    char *act_titulo=array_et_course_title[id_course];
    return act_titulo;
}
void error(const char *s){
    /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
    perror (s);
    exit(EXIT_FAILURE);
}
