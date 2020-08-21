typedef struct{
    int id;
    char et_enunciado[100];
    int num_opciones;
    char *et_respuesta[10];

} Pregunta;

Pregunta preguntas[]={
    {
        0,
        "Mecacurses - Elige una opción",
        3,
         {  "Curso de mecanografia",
            "Practicar con texto personalizado",
            "Configurar"
             
        }
    },
    {
        1,
        "Te gustan los espaguetis?",
        4,
         {
            "Que son los espaguetis?",
            "Lo que me gusta es.",
            "No lo sé",
            "Sí a ti te gustan a mi también"
        }
    }

};
