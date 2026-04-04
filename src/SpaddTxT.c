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
    Editor *active_editor=NULL;

    init_window_manager(&wm);
    
    active_editor=getActiveTileEditor(&wm);
     active_window=getActiveTileWindow(&wm);
     active_tile=wm.active_tile;
   

    //TODO-handle changing windows and active tiles
    //TODO-handle when editor is closed, close tile, and if no tiles left, close window and exit program
    while(active_editor->running) { //CURRENTLY ONLY ONE TILE, SO THIS IS EFFECTIVELY THE MAIN LOOP
        int ch=wgetch(active_window);
        input_handle_key(active_editor, ch);
        if(active_editor->running==0) {     //CHECK TO ENSURE STILL RUNNING AFTER INPUT
            break;
        }
         //TODO-resize active tile, not just first tile
         if(ch=='r')  //TEST- RESIZE TILE ON R KEY
          resizeTile(&wm, 0, 20, 20);
        
       
        tile_render(active_tile);
  //  wrefresh(active_window);
    }
        

    shutdown_window_manager(&wm);

    endwin(); /* End curses mode and restore terminal */
   // delwin();
    return 0;
}
