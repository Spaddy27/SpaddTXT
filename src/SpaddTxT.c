#include <ncurses.h>
#include <locale.h>

#include "editor/editor.h"
#include "input/input.h"
#include "screen/screen.h"

#include <stdio.h>


int main(void) {
    //INTITIALIZATION FOR NCURSES
    setlocale(LC_ALL, "");
    initscr();
    //	cbreak();               /* Disable line buffering */
    noecho(); /* Don't echo() while we do getch */
    keypad(stdscr, TRUE); /* Enable function keys like F1, arrow keys, etc. */
    scrollok(stdscr, TRUE); //ENABLE BOTTOM OF WINDOW SCROLL
    //Create Components
    Editor editor;
    //  Input in;
    //   Screen screen;

    //Initialize the editor
    editor_init(&editor);
    initScreen(&editor);


    //MAIN LOOP
    while (editor.running) {
        int key = getch();
        input_handle_key(&editor, key);
        if (editor.running)
            screen_render(&editor);
    }


    endwin(); /* End curses mode and restore terminal */
   // delwin();
    return 0;
}
