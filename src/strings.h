#ifndef FOO_STRINGS_H    /* This is an "include guard" */
#define FOO_STRINGS_H    /* prevents the file from being included twice. */
//main.c
#define ET_ERRORS "Errors"
#define ET_DESC "Repeat the text you see below:"
#define ET_OPTION0 "Exit"
#define ET_OPTION1 "Tutorial"
#define ET_OPTION21 "Cancel Lesson"
#define ET_OPTION22 "Repeat Lesson"
#define ET_OPTION3 "Change Lesson"
#define ET_OPTION4 "Change Course"
#define ET_TIME "Time"
#define ET_TIME_OVER "TIME OVER!!"
#define ET_PRESS_KEY_CONTINUE "Press any key to continue..."
#define ET_YOUR_SCORE "You've obtained the following score:"
#define ET_PPM "Pulses/min"
#define ET_LESSON "Lesson"
#define ET_FINISH_PHRASE1 "Congratulations! You have done it perfect! :D\nYou made 0 errors!\nThe next lesson is waiting for you Mr. Keyman"
#define ET_FINISH_PHRASE2 "You made %d errors! :)\nNow you are ready for go to the next lesson, but need improve your accuracy"
#define ET_FINISH_PHRASE3 "You made %d errors.\nYou need repeat this lesson until do only 3 or less errors"
#define ET_FINISH_PHRASE_CANCELLED "This practice has been canceled"

//menus.h
#define ET_PROGRAMA "LTYPER v1.0"
#define ET_ENUNCIADO_COURSE "Choose the Course you want to take:"
#define ET_ENUNCIADO_LESSON "Choose the Lesson you want to practice:"


//json_reader.h
char *array_et_course_title[6]={
    0,
    "Typing course QWERTY",
    "Typing course DVORAK",
    "Typing course COLEMAK",
    "Typing course DVORAK REVERSE"
};

//draw_keys.h
#define ET_FINGER_INDEX "Index finger"
#define ET_FINGER_MIDDLE "Middle finger"
#define ET_FINGER_RING "Ring finger"
#define ET_FINGER_LITTLE "Little finger"
#define ET_FINGER_THUMB "Thumb."

//introduction.h
#define ET_INTRODUCTION_1 "In this digital age the keyboard is one of the most common media used for comunicate us, and write without errors and fast will do faster the comunications. Typing is an skill that is learned with practice. We save much time if we learn something basic like master the keyboard."
#define ET_INTRODUCTION_2 "By basic and repetitive exercises we will acquire practice and speed. We could must reach between 70-250 pulses/minute. We will must try do the minimum errors as possible."
#define ET_INTRODUCTION_3 "All the time your hands should be positioned correctly, and you should avoid vocalize the letters with your mouth when you write. Only you can repeat them mentally."
#define ET_INTRODUCTION_4 "WRITE CORRECTLY IS MOST IMPORTANT THAN TO WRITE TOO FAST! Where 3% might be the maximum allowed errors before go to the next lesson."
#define ET_INTRODUCTION_5 "For a better program visualization, recommend taking the courses with the window maximized."
#define ET_INTRODUCTION_6 "Always the hands will stay in the home position, it means that the fingers index, middle, ring and little stay above the ASDF and JKL; keys in a QWERTY keyboard. The thumbs will rest on the space key. Each time a key is pressed the hands will return to the home position."
#define ET_PRESS_ANY_KEY "Press any key to continue or ESC to exit the program"
#define ET_INTRODUCTION_TITLE "Tutorial"

#endif /* FOO_STRINGS H */
