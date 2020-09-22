//main.c
#define ET_ERRORS "Errores"
#define ET_DESC "Repite el texto que ves a continuación:"
#define ET_OPTION0 "Salir"
#define ET_OPTION11 "Cancelar texto"
#define ET_OPTION12 "Repetir texto"
#define ET_OPTION2 "Cambiar de lección"
#define ET_OPTION3 "Cambiar de curso"
#define ET_TIME "Tiempo"
#define ET_TIME_OVER "SE TE HA ACABADO EL TIEMPO!!"
#define ET_PRESS_KEY_CONTINUE "Presiona cualquier tecla para continuar..."
#define ET_YOUR_SCORE "Has obtenido la siguiente puntuación:"
#define ET_PPM "Pulsaciones/min"
#define ET_LESSON "Lección"


//menus.h
#define ET_PROGRAMA "TYPEMINAL v0.1"
#define ET_ENUNCIADO_COURSE "Escoge el curso que quieres realizar:"
#define ET_ENUNCIADO_LESSON "Escoge la lección que quieres practicar:"


//json_reader.h
char *array_et_course_title[MAX_COURSES]={
    0,
    "Curso de mecanografia en inglés (dist. QWERTY)",
    "Curso de mecanografia en inglés (dist. DVORAK)",
    "Curso de mecanografia en inglés (dist. COLEMAK)",
    "Curso de mecanografia en inglés (dist. DVORAK REVERSE)"
};

    // NOMBRES DE LOS DEDOS EN INGLÉS
    // Index finger. el dedo índice.
    // Middle finger. el dedo corazón.
    // Ring finger. el anular.
    // Little finger. el meñique.
    // Thumb. el pulgar.

//draw_keys.h
#define ET_FINGER_INDEX "Dedo índice"
#define ET_FINGER_MIDDLE "Dedo corazón"
#define ET_FINGER_RING "Dedo anular"
#define ET_FINGER_LITTLE "Dedo meñique"
#define ET_FINGER_THUMB "Dedo pulgar"

//introduction.h
#define ET_INTRODUCTION_1 "Hoy en dia el teclado es uno de los medios utilizados para comunicarnos y escribir correctamente y sin errores con él agilizará las comunicaciones. La mecanografía es una habilidad que se aprende con la práctica.Nos ahorraremos mucho tiempo si aprendemos algo básico como dominar el teclado."

#define ET_INTRODUCTION_2 "Mediante ejercicios básicos y repetitivos iremos adquiriendo práctica y también una velocidad con la que deberiamos alcanzar entre 70 y 250 pulsaciones por minuto. Deberemos intentar hacer el mínimo posible de errores siendo 3 el número máximo de errores permitidos antes de saltar a la siguiente lección."

#define ET_INTRODUCTION_3 "Las manos siempre estarán en la posición Base, esto quiere decir que los dedos índice, corazón, anular y meñique irán sobre las teclas ASDF y JKL; en el teclado QWERTY. Los pulgares descansarán sobre la tecla espacio. Cada vez que se pulse una tecla las manos volverán a la posición base."

#define ET_PRESS_ANY_KEY "Pulse cualquier tecla para continuar o ESC para salir del programa"

#define ET_INTRODUCTION_TITLE "Introducción"
