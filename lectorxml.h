// #include "header.h"

//para probar este archivo lo compilamos y descomentamos esta función
// int main(int argc, char **argv) {
//     char *array_texto;
//     array_texto=obten_texto(0);
//     
//     printf("obten (%d)", *(array_texto+0));
// //     );
// /*
//     int i=0;
//     while( array_texto[i]!= NULL) {
//         printf("i:%d (%s)\n", i,array_texto[i]);        
//         i++;
//     }*/
// }
char *obten_texto(int id_texto){
    xmlDoc *document;
    xmlNode *root, *node;
//     xmlChar *act_texto;
    char *act_texto;
    int long_texto, i=0, i_row=0;
// //     const static char **array_todos_textos = malloc(80 * sizeof(char));
    document = xmlReadFile("db_texts.xml", NULL, 0);
    root = xmlDocGetRootElement(document);
    
    for (node = root->children; node; node = node->next) {
        /* Si el nodo hijo es de texto y no tiene contenido no lo almacenaremos... */
        if(!((node->type==3) && (strlen(trim((char*)xmlNodeGetContent(node)))==0))){
            if(i==id_texto){
                act_texto=trim((char*)xmlNodeGetContent(node));
            }
        i++;
        }
    }
    
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
    //
    char *array_devolver;
    array_devolver=&tmp_cadena[0];//devolveremos el puntero de donde empieza tmp_cadena    
    return array_devolver;
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
