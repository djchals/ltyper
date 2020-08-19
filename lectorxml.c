#include "header.h"

// int main(int argc, char **argv) {
//     obten_texto(0);
// 
//     int i=0;
//     while( array_texto[i]!= NULL) {
//         printf("i:%d (%s)\n", i,array_texto[i]);        
//         i++;
//     }
// }
void obten_texto(int id_texto){
// char *obten_texto(int id_texto){
    char **array_todos_textos=obten_todos_textos();
    int long_texto, i, i_row=0;
    
    char tmp_cadena[255]={};

    long_texto=strlen(array_todos_textos[id_texto]);

    //quitamos los espacios de sobra
    for(i=0;i<long_texto;i++){
        switch(array_todos_textos[0][i]){
            case 0x20://es un espacio
                if(array_todos_textos[0][i+1]!=0x20){//si lo siguiente no es un espacio o un salto de línea, ni tampoco estamos al principio de la linea lo sumaremos
                    tmp_cadena[i_row]=array_todos_textos[id_texto][i];
                    i_row++;
                }
                break;
            default://es una letra normal
                    tmp_cadena[i_row]=array_todos_textos[id_texto][i];
                    i_row++;
                break;
        }
    }

    const char s[2] = "\x0a";//código de la tecla enter
    char *token;
    
    /* get the first token */
    i=0;
    token = strtok(tmp_cadena, s);

    /* walk through other tokens */
    while( token != NULL ) {
        array_texto[i]=token;
        token = strtok(NULL, s);
        i++;
    }
    array_texto[i]=NULL;//El último lo llenamos con null para saber que acaba aquí el array
}


char *trim(char *s){
    char *start = s;

    /* Nos comemos los espacios al inicio */
    while(*start && isspace(*start))
        ++start;

    char *i = start;
    char *end = start;

    /* Nos comemos los espacios al final */
    while(*i) {
        if(!isspace(*(i++)) )
        end = i;
    }

    /* Escribimos el terminados */
    *end = 0;
    return start;
}


char **obten_todos_textos(void){
    xmlDoc *document;
    xmlNode *root, *node;
//     xmlChar *value;

    int i=0;
    char **array_todos_textos = malloc(80 * sizeof(char));

    document = xmlReadFile("archivo.xml", NULL, 0);
    root = xmlDocGetRootElement(document);
    
    for (node = root->children; node; node = node->next) {
        /* Si el nodo hijo es de texto y no tiene contenido no lo almacenaremos... */
        if(!((node->type==3) && (strlen(trim((char*)xmlNodeGetContent(node)))==0))){
            array_todos_textos[i]=trim((char*)xmlNodeGetContent(node));
//         printf ("\ni=%d, dentro obten array: %s",i,array_todos_textos[i]);
            i++;
        }
    }
    return array_todos_textos;
}
