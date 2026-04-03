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
    cbreak(); /* Line buffering disabled, Pass on everty thing to me */
    keypad(stdscr, TRUE); /* Enable function keys like F1, arrow keys, etc. */
    //scrollok(stdscr, TRUE); //ENABLE BOTTOM OF WINDOW SCROLL
     
    //CREATE WINDOW MANAGER
    Window_manager wm;
    WINDOW *active_window=NULL;
    Tile *active_tile=NULL;
    Editor *editor=NULL;

    init_window_manager(&wm);
    active_tile=wm.active_tile;
    active_window=active_tile->window;
    editor=active_tile->editor;

    
    while(wm.running) {
        int ch=wgetch(active_window);
        input_handle_key(editor, ch);
    //    screen_render(active_tile->editor);
        tile_render(active_tile);
       // ch=wgetch(active_window);
    }
          
    



    endwin(); /* End curses mode and restore terminal */
   // delwin();
    return 0;
}
