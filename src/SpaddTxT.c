#include <ncurses.h>
#include <locale.h>

#include "editor/editor.h"
#include "input/input.h"
#include "screen/screen.h"
#include "screen/window_manager.h"
#include "screen/tile.h"
#include <stdio.h>


int main(void) {
    //INTITIALIZATION FOR NCURSES
    setlocale(LC_ALL, "");
    initscr();
    noecho(); /* Don't echo() while we do getch */
    //keypad(stdscr, TRUE); /* Enable function keys like F1, arrow keys, etc. */
    //scrollok(stdscr, TRUE); //ENABLE BOTTOM OF WINDOW SCROLL
     
    //CREATE WINDOW MANAGER
    Window_manager wm;
          
    init_window_manager(&wm);
                           

    endwin(); /* End curses mode and restore terminal */
   // delwin();
    return 0;
}
