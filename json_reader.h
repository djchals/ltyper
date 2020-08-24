// #include <stdio.h>
// #include <stdlib.h>
// #include <signal.h> // Para las constantes SIGALRM y similares
// #include <unistd.h> //incluyo este para tener la funcion sleep()
// #include <string.h> //incluyo para tener strcpy()
// #include <math.h> //necesitaremos floor() para el cronometro
// #include<json-c/json.h>
// #include <ctype.h>

char *obten_texto(int id_texto);
char *obten_titulo(int id_texto);
int obten_num_titulos();
void obten_titulos();

void quita_espacios(char *act_texto);

json_object *parsed_json, *titulos;

void _init_json();

// int main(int argc, char **argv) {
//     _init_json();
//     //     obten_titulos();
//     //     obten_texto(0);
// 
//     n_titulos = obten_num_titulos();
//     //         ;
//     m_lecciones[0]=obten_titulo(0);
//         m_lecciones[1]=obten_titulo(1);
//     printf("título aquí: %s \n", m_lecciones[0]);
// 
//     printf("título aquí: %s \n", m_lecciones[1]);
// 
//     // 	printf("Encontrados %d titulos\n",obten_num_titulos());
//     return 0;
// }

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
// 	int n_textos = json_object_array_length(textos);

    texto = json_object_array_get_idx(textos, id_texto);
    char *act_texto=(char *) json_object_get_string(texto);
    quita_espacios(act_texto);
    return act_texto;
}
void quita_espacios(char *act_texto){
    int long_texto, i=0, i_row=0;
    long_texto=strlen(act_texto);
    char tmp_cadena[255];

    //quitamos los espacios de sobra
    for(i=0;i<long_texto;i++){
        switch(act_texto[i]){
            case 0x20://es un espacio
                if(act_texto[i+1]!=0x20 && act_texto[i+1]!=0x0a){//si lo siguiente no es un espacio o un salto de línea, ni tampoco estamos al principio de la linea lo sumaremos
                    tmp_cadena[i_row]=act_texto[i];
                    i_row++;
                }
                break;
            default://es una letra normal
                    tmp_cadena[i_row]=act_texto[i];
                    i_row++;
                break;
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
